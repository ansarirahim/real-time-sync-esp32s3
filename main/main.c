/**
 * @file main.c
 * @brief ESP-NOW RTC Synchronization - Main Application
 *
 * Main application entry point for ESP32-S3 wireless sensor network with
 * RTC-based time synchronization. Implements both gateway and sensor node
 * roles with deep sleep power management and ESP-NOW communication.
 *
 * @details
 * - Gateway Mode: Broadcasts time sync, receives sensor data, manages network
 * - Sensor Mode: Wakes on RTC alarm, syncs time, transmits data, sleeps
 * - Power Management: Deep sleep with RTC alarm wake (< 20 µA)
 * - Communication: ESP-NOW protocol for low-latency wireless sync
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
 *
 * @note Target: ESP32-S3, Framework: ESP-IDF v5.5.1+
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

// Component includes
#include "rtc_rv3028.h"
#include "espnow_sync.h"
#include "sleep_manager.h"

static const char *TAG = "MAIN";

// Configuration - Change these based on device role
#define DEVICE_ROLE         ESPNOW_ROLE_SENSOR  // Change to ESPNOW_ROLE_GATEWAY for gateway
#define SENSOR_ID           1                    // Sensor node ID (1, 2, or 3)
#define WAKE_INTERVAL_SEC   3600                 // 1 hour wake interval
#define WIFI_CHANNEL        1                    // WiFi channel for ESP-NOW

// GPIO Pin Definitions (from Milestone 1 documentation)
#define GPIO_RTC_SDA        GPIO_NUM_8
#define GPIO_RTC_SCL        GPIO_NUM_9
#define GPIO_RTC_INT        GPIO_NUM_3
#define GPIO_BUTTON         GPIO_NUM_4  // Gateway only

// Gateway MAC address (update with actual gateway MAC)
static uint8_t gateway_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/**
 * @brief Time sync callback (sensor nodes)
 */
static void on_time_sync_received(const espnow_time_sync_t *sync_data) {
    ESP_LOGI(TAG, "Time sync received!");
    ESP_LOGI(TAG, "  Timestamp: %lu", (unsigned long)sync_data->timestamp);
    ESP_LOGI(TAG, "  Next wake: %lu", (unsigned long)sync_data->next_wake_time);
    ESP_LOGI(TAG, "  Interval: %d seconds", sync_data->wake_interval);

    // Update RTC with synchronized time
    rtc_set_unix_time(sync_data->timestamp);

    // Set alarm for next wake
    rtc_set_alarm(sync_data->next_wake_time);
}

/**
 * @brief Sensor data callback (gateway)
 */
static void on_sensor_data_received(const espnow_sensor_data_t *sensor_data) {
    ESP_LOGI(TAG, "Sensor data received!");
    ESP_LOGI(TAG, "  Sensor ID: %d", sensor_data->sensor_id);
    ESP_LOGI(TAG, "  Timestamp: %lu", (unsigned long)sensor_data->timestamp);
    ESP_LOGI(TAG, "  Data count: %d bytes", sensor_data->data_count);

    // Process sensor data here
    // TODO: Store to SD card, send to cloud, etc.
}

/**
 * @brief Main application entry point
 */
void app_main(void)
{
    ESP_LOGI(TAG, "===========================================");
    ESP_LOGI(TAG, "ESP-NOW RTC Synchronization Project");
    ESP_LOGI(TAG, "Version: 1.0.0-M2 (Milestone 2)");
    ESP_LOGI(TAG, "Target: ESP32-S3");
    ESP_LOGI(TAG, "Role: %s", DEVICE_ROLE == ESPNOW_ROLE_GATEWAY ? "GATEWAY" : "SENSOR");
    if (DEVICE_ROLE == ESPNOW_ROLE_SENSOR) {
        ESP_LOGI(TAG, "Sensor ID: %d", SENSOR_ID);
    }
    ESP_LOGI(TAG, "===========================================");

    // Print wake-up reason
    sleep_print_wake_reason();
    wake_source_t wake_source = sleep_get_wake_source();

    // Initialize NVS (required for WiFi/ESP-NOW)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS initialized");

    // Print chip information
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "Chip: %s with %d CPU cores, WiFi%s%s",
             CONFIG_IDF_TARGET,
             chip_info.cores,
             (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
             (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    ESP_LOGI(TAG, "Silicon revision: %d", chip_info.revision);

    // Get flash size
    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, &flash_size);
    ESP_LOGI(TAG, "Flash: %luMB %s",
             flash_size / (1024 * 1024),
             (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    ESP_LOGI(TAG, "Free heap: %lu bytes", esp_get_free_heap_size());

    // Initialize RTC driver
    ESP_LOGI(TAG, "Initializing RTC driver...");
    rtc_config_t rtc_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_pin = GPIO_RTC_SDA,
        .scl_pin = GPIO_RTC_SCL,
        .int_pin = GPIO_RTC_INT,
        .i2c_freq_hz = 100000
    };
    ESP_ERROR_CHECK(rtc_rv3028_init(&rtc_cfg));

    if (rtc_is_available()) {
        ESP_LOGI(TAG, "RTC hardware detected!");

        // Read current time
        uint32_t timestamp;
        if (rtc_get_unix_time(&timestamp) == ESP_OK) {
            ESP_LOGI(TAG, "RTC timestamp: %lu", (unsigned long)timestamp);
        }
    } else {
        ESP_LOGW(TAG, "RTC hardware not detected - using stub mode");
        ESP_LOGW(TAG, "Connect RV-3028-C7 for full functionality");
    }

    // Initialize sleep manager
    ESP_LOGI(TAG, "Initializing sleep manager...");
    sleep_config_t sleep_cfg = {
        .rtc_int_pin = GPIO_RTC_INT,
        .button_pin = GPIO_BUTTON,
        .enable_button_wake = (DEVICE_ROLE == ESPNOW_ROLE_GATEWAY),
        .default_sleep_sec = WAKE_INTERVAL_SEC
    };
    ESP_ERROR_CHECK(sleep_manager_init(&sleep_cfg));

    // Initialize ESP-NOW
    ESP_LOGI(TAG, "Initializing ESP-NOW...");
    espnow_config_t espnow_cfg = {
        .role = DEVICE_ROLE,
        .channel = WIFI_CHANNEL,
        .sensor_id = SENSOR_ID,
        .wake_interval = WAKE_INTERVAL_SEC
    };
    memcpy(espnow_cfg.gateway_mac, gateway_mac, 6);
    ESP_ERROR_CHECK(espnow_sync_init(&espnow_cfg));

    // Register callbacks
    if (DEVICE_ROLE == ESPNOW_ROLE_SENSOR) {
        espnow_register_time_sync_cb(on_time_sync_received);
    } else {
        espnow_register_sensor_data_cb(on_sensor_data_received);
    }

    ESP_LOGI(TAG, "===========================================");
    ESP_LOGI(TAG, "All components initialized successfully!");
    ESP_LOGI(TAG, "===========================================");

    // Main application logic
    if (DEVICE_ROLE == ESPNOW_ROLE_GATEWAY) {
        // Gateway mode: stay awake and handle sensor data
        ESP_LOGI(TAG, "Gateway mode: Listening for sensor data...");

        uint32_t loop_count = 0;
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(10000));  // 10 seconds

            loop_count++;
            ESP_LOGI(TAG, "[%lu] Gateway running, free heap: %lu bytes",
                     (unsigned long)loop_count, esp_get_free_heap_size());

            // Send time sync broadcast every minute
            if (loop_count % 6 == 0) {  // Every 60 seconds
                uint32_t current_time = 1700000000 + (loop_count * 10);  // Stub timestamp
                uint32_t next_wake = current_time + WAKE_INTERVAL_SEC;
                espnow_send_time_sync(current_time, next_wake);
            }
        }
    } else {
        // Sensor mode: wake, measure, transmit, sleep
        ESP_LOGI(TAG, "Sensor mode: Wake -> Measure -> Transmit -> Sleep");

        // Simulate sensor reading
        uint8_t sensor_data[10];
        for (int i = 0; i < 10; i++) {
            sensor_data[i] = i + SENSOR_ID;
        }

        // Wait for time sync (timeout after 5 seconds)
        ESP_LOGI(TAG, "Waiting for time sync from gateway...");
        int timeout = 50;  // 5 seconds
        while (!espnow_is_time_synced() && timeout > 0) {
            vTaskDelay(pdMS_TO_TICKS(100));
            timeout--;
        }

        if (espnow_is_time_synced()) {
            ESP_LOGI(TAG, "Time synchronized!");

            // Send sensor data
            uint32_t timestamp = espnow_get_last_timestamp();
            espnow_send_sensor_data(sensor_data, sizeof(sensor_data), timestamp);
            ESP_LOGI(TAG, "Sensor data transmitted");

            // Wait for ACK
            vTaskDelay(pdMS_TO_TICKS(500));
        } else {
            ESP_LOGW(TAG, "Time sync timeout - will retry on next wake");
        }

        // Enter deep sleep
        ESP_LOGI(TAG, "Entering deep sleep for %d seconds...", WAKE_INTERVAL_SEC);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Allow logs to flush
        sleep_for_seconds(WAKE_INTERVAL_SEC);
    }
}

