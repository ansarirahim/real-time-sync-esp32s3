/**
 * @file sleep_manager.h
 * @brief Deep Sleep Management for ESP32
 * 
 * @author Abdul Raheem Ansari
 * @email ansarirahim1@gmail.com
 * @date 2024-11-18
 * 
 * This component manages deep sleep modes with RTC wake-up and button wake-up.
 */

#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_sleep.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Wake-up source types
 */
typedef enum {
    WAKE_SOURCE_UNKNOWN = 0,
    WAKE_SOURCE_RTC_ALARM,      ///< Woke up from RTC alarm
    WAKE_SOURCE_BUTTON,         ///< Woke up from button press
    WAKE_SOURCE_TIMER,          ///< Woke up from timer
    WAKE_SOURCE_RESET           ///< Power-on reset
} wake_source_t;

/**
 * @brief Sleep configuration
 */
typedef struct {
    gpio_num_t rtc_int_pin;     ///< RTC interrupt pin (ext0)
    gpio_num_t button_pin;      ///< Button pin (ext1, gateway only)
    bool enable_button_wake;    ///< Enable button wake (gateway only)
    uint32_t default_sleep_sec; ///< Default sleep duration (seconds)
} sleep_config_t;

/**
 * @brief Initialize sleep manager
 * 
 * @param config Sleep configuration
 * @return esp_err_t ESP_OK on success
 */
esp_err_t sleep_manager_init(const sleep_config_t *config);

/**
 * @brief Enter deep sleep until RTC alarm
 * 
 * @param wake_time_sec Wake time in seconds from now
 * @return esp_err_t ESP_OK on success (won't return if successful)
 */
esp_err_t sleep_until_rtc_alarm(uint32_t wake_time_sec);

/**
 * @brief Enter deep sleep until absolute timestamp
 * 
 * @param wake_timestamp Unix timestamp to wake up
 * @param current_timestamp Current Unix timestamp
 * @return esp_err_t ESP_OK on success (won't return if successful)
 */
esp_err_t sleep_until_timestamp(uint32_t wake_timestamp, uint32_t current_timestamp);

/**
 * @brief Enter deep sleep with timer wake-up
 * 
 * @param sleep_sec Sleep duration in seconds
 * @return esp_err_t ESP_OK on success (won't return if successful)
 */
esp_err_t sleep_for_seconds(uint32_t sleep_sec);

/**
 * @brief Get wake-up source
 * 
 * @return wake_source_t Wake-up source
 */
wake_source_t sleep_get_wake_source(void);

/**
 * @brief Get wake-up source as string
 * 
 * @return const char* Wake-up source name
 */
const char* sleep_get_wake_source_name(void);

/**
 * @brief Print wake-up reason
 */
void sleep_print_wake_reason(void);

/**
 * @brief Get total sleep time (microseconds)
 * 
 * @return uint64_t Total sleep time
 */
uint64_t sleep_get_total_sleep_time_us(void);

/**
 * @brief Check if this is first boot
 * 
 * @return true if first boot (not wake from sleep)
 */
bool sleep_is_first_boot(void);

#ifdef __cplusplus
}
#endif

#endif // SLEEP_MANAGER_H

