/**
 * @file sleep_manager.c
 * @brief Deep Sleep Management Implementation for ESP32
 *
 * This file implements deep sleep power management with RTC alarm wake (ext0)
 * and button wake (ext1) support. Provides absolute timestamp scheduling to
 * prevent clock drift accumulation during sleep cycles.
 *
 * @author A.R. Ansari
 * @email ansarirahim1@gmail.com
 * @date November 18, 2024
 * @version 1.0.0
 *
 * @contact
 * WhatsApp: +919024304833
 * LinkedIn: https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/
 *
 * @copyright Copyright (c) 2024 A.R. Ansari. All rights reserved.
 */

#include "sleep_manager.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "driver/rtc_io.h"
#include <string.h>

static const char *TAG = "SLEEP_MGR";

static sleep_config_t sleep_config;
static bool sleep_initialized = false;

esp_err_t sleep_manager_init(const sleep_config_t *config) {
    if (config == NULL) {
        ESP_LOGE(TAG, "Config is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Initializing sleep manager");
    ESP_LOGI(TAG, "RTC INT pin: GPIO%d, Button pin: GPIO%d, Button wake: %s",
             config->rtc_int_pin, config->button_pin,
             config->enable_button_wake ? "enabled" : "disabled");

    memcpy(&sleep_config, config, sizeof(sleep_config_t));

    // Configure RTC GPIO for ext0 wake (RTC alarm interrupt)
    if (rtc_gpio_is_valid_gpio(config->rtc_int_pin)) {
        ESP_ERROR_CHECK(rtc_gpio_init(config->rtc_int_pin));
        ESP_ERROR_CHECK(rtc_gpio_set_direction(config->rtc_int_pin, RTC_GPIO_MODE_INPUT_ONLY));
        ESP_ERROR_CHECK(rtc_gpio_pulldown_en(config->rtc_int_pin));
        ESP_ERROR_CHECK(rtc_gpio_pullup_dis(config->rtc_int_pin));
        
        // Enable ext0 wake on HIGH level (RTC alarm pulls pin HIGH)
        ESP_ERROR_CHECK(esp_sleep_enable_ext0_wakeup(config->rtc_int_pin, 1));
        ESP_LOGI(TAG, "ext0 wake enabled on GPIO%d (RTC alarm)", config->rtc_int_pin);
    } else {
        ESP_LOGW(TAG, "RTC INT pin GPIO%d is not RTC-capable", config->rtc_int_pin);
    }

    // Configure button GPIO for ext1 wake (gateway only)
    if (config->enable_button_wake && rtc_gpio_is_valid_gpio(config->button_pin)) {
        ESP_ERROR_CHECK(rtc_gpio_init(config->button_pin));
        ESP_ERROR_CHECK(rtc_gpio_set_direction(config->button_pin, RTC_GPIO_MODE_INPUT_ONLY));
        ESP_ERROR_CHECK(rtc_gpio_pulldown_en(config->button_pin));
        ESP_ERROR_CHECK(rtc_gpio_pullup_dis(config->button_pin));
        
        // Enable ext1 wake on HIGH level (button press pulls pin HIGH)
        uint64_t button_mask = (1ULL << config->button_pin);
        ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup(button_mask, ESP_EXT1_WAKEUP_ANY_HIGH));
        ESP_LOGI(TAG, "ext1 wake enabled on GPIO%d (button)", config->button_pin);
    }

    sleep_initialized = true;
    ESP_LOGI(TAG, "Sleep manager initialized successfully");
    
    return ESP_OK;
}

esp_err_t sleep_for_seconds(uint32_t sleep_sec) {
    if (!sleep_initialized) {
        ESP_LOGE(TAG, "Sleep manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Entering deep sleep for %lu seconds", (unsigned long)sleep_sec);
    
    // Enable timer wake-up
    esp_sleep_enable_timer_wakeup(sleep_sec * 1000000ULL);
    
    // Enter deep sleep
    esp_deep_sleep_start();
    
    // This line will never be reached
    return ESP_OK;
}

esp_err_t sleep_until_rtc_alarm(uint32_t wake_time_sec) {
    if (!sleep_initialized) {
        ESP_LOGE(TAG, "Sleep manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Entering deep sleep until RTC alarm (in %lu seconds)",
             (unsigned long)wake_time_sec);
    
    // ext0 is already configured in init
    // RTC alarm will trigger interrupt on rtc_int_pin
    
    // Also enable timer as backup
    esp_sleep_enable_timer_wakeup(wake_time_sec * 1000000ULL);
    
    // Enter deep sleep
    esp_deep_sleep_start();
    
    // This line will never be reached
    return ESP_OK;
}

esp_err_t sleep_until_timestamp(uint32_t wake_timestamp, uint32_t current_timestamp) {
    if (!sleep_initialized) {
        ESP_LOGE(TAG, "Sleep manager not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (wake_timestamp <= current_timestamp) {
        ESP_LOGW(TAG, "Wake timestamp is in the past or now");
        return ESP_ERR_INVALID_ARG;
    }

    uint32_t sleep_duration = wake_timestamp - current_timestamp;
    
    ESP_LOGI(TAG, "Sleeping until timestamp %lu (current: %lu, duration: %lu sec)",
             (unsigned long)wake_timestamp,
             (unsigned long)current_timestamp,
             (unsigned long)sleep_duration);
    
    return sleep_until_rtc_alarm(sleep_duration);
}

wake_source_t sleep_get_wake_source(void) {
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    
    switch (cause) {
        case ESP_SLEEP_WAKEUP_EXT0:
            return WAKE_SOURCE_RTC_ALARM;
        
        case ESP_SLEEP_WAKEUP_EXT1:
            return WAKE_SOURCE_BUTTON;
        
        case ESP_SLEEP_WAKEUP_TIMER:
            return WAKE_SOURCE_TIMER;
        
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            return WAKE_SOURCE_RESET;
    }
}

const char* sleep_get_wake_source_name(void) {
    wake_source_t source = sleep_get_wake_source();

    switch (source) {
        case WAKE_SOURCE_RTC_ALARM:
            return "RTC Alarm";
        case WAKE_SOURCE_BUTTON:
            return "Button Press";
        case WAKE_SOURCE_TIMER:
            return "Timer";
        case WAKE_SOURCE_RESET:
            return "Power-On Reset";
        default:
            return "Unknown";
    }
}

void sleep_print_wake_reason(void) {
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

    ESP_LOGI(TAG, "===========================================");
    ESP_LOGI(TAG, "Wake-up source: %s", sleep_get_wake_source_name());

    switch (cause) {
        case ESP_SLEEP_WAKEUP_EXT0:
            ESP_LOGI(TAG, "Wake from ext0 (RTC alarm on GPIO%d)", sleep_config.rtc_int_pin);
            break;

        case ESP_SLEEP_WAKEUP_EXT1: {
            uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();
            if (wakeup_pin_mask != 0) {
                int pin = __builtin_ffsll(wakeup_pin_mask) - 1;
                ESP_LOGI(TAG, "Wake from ext1 (button on GPIO%d)", pin);
            }
            break;
        }

        case ESP_SLEEP_WAKEUP_TIMER:
            ESP_LOGI(TAG, "Wake from timer");
            break;

        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            ESP_LOGI(TAG, "Wake from power-on reset or other source");
            break;
    }

    ESP_LOGI(TAG, "===========================================");
}

uint64_t sleep_get_total_sleep_time_us(void) {
    // In ESP-IDF v5.5+, use esp_timer_get_time() instead of esp_rtc_get_time_us()
    return esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED
           ? esp_timer_get_time()
           : 0;
}

bool sleep_is_first_boot(void) {
    return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED;
}

