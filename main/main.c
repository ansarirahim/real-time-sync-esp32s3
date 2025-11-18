/**
 * ESP-NOW RTC Synchronization Project
 * Main Application Entry Point
 * 
 * Milestone 1: Project Setup and Hardware Analysis
 * 
 * Target: ESP32-S3
 * Framework: ESP-IDF v5.x
 * 
 * Author: Abdul Raheem Ansari
 * Date: 2024-11-18
 * Version: 1.0.0-M1
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

static const char *TAG = "MAIN";

/**
 * @brief Main application entry point
 */
void app_main(void)
{
    ESP_LOGI(TAG, "===========================================");
    ESP_LOGI(TAG, "ESP-NOW RTC Synchronization Project");
    ESP_LOGI(TAG, "Version: 1.0.0-M1 (Milestone 1)");
    ESP_LOGI(TAG, "Target: ESP32-S3");
    ESP_LOGI(TAG, "===========================================");

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
    ESP_LOGI(TAG, "Flash: %dMB %s",
             spi_flash_get_chip_size() / (1024 * 1024),
             (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // Print free heap
    ESP_LOGI(TAG, "Free heap: %lu bytes", esp_get_free_heap_size());

    ESP_LOGI(TAG, "===========================================");
    ESP_LOGI(TAG, "Milestone 1: Hardware Analysis & Setup");
    ESP_LOGI(TAG, "Status: Project initialized successfully");
    ESP_LOGI(TAG, "Next: RTC component integration");
    ESP_LOGI(TAG, "===========================================");

    // Main loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "System running... Free heap: %lu bytes", esp_get_free_heap_size());
    }
}

