/**
 * @file rtc_rv3028.h
 * @brief RV-3028-C7 Real-Time Clock Driver for ESP32
 *
 * This driver provides interface for Micro Crystal RV-3028-C7 ultra-low power
 * real-time clock module with I2C interface. Features include time management,
 * alarm configuration, Unix timestamp support, and interrupt handling.
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

#ifndef RTC_RV3028_H
#define RTC_RV3028_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/i2c.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RV-3028-C7 I2C Address
 */
#define RV3028_I2C_ADDR         0x52

/**
 * @brief RV-3028-C7 Register Addresses
 */
#define RV3028_REG_SECONDS      0x00
#define RV3028_REG_MINUTES      0x01
#define RV3028_REG_HOURS        0x02
#define RV3028_REG_WEEKDAY      0x03
#define RV3028_REG_DATE         0x04
#define RV3028_REG_MONTH        0x05
#define RV3028_REG_YEAR         0x06
#define RV3028_REG_UNIX_TIME0   0x1B
#define RV3028_REG_UNIX_TIME1   0x1C
#define RV3028_REG_UNIX_TIME2   0x1D
#define RV3028_REG_UNIX_TIME3   0x1E
#define RV3028_REG_CONTROL1     0x0F
#define RV3028_REG_CONTROL2     0x10
#define RV3028_REG_STATUS       0x0E

/**
 * @brief RTC Time Structure
 */
typedef struct {
    uint8_t seconds;    ///< Seconds (0-59)
    uint8_t minutes;    ///< Minutes (0-59)
    uint8_t hours;      ///< Hours (0-23)
    uint8_t weekday;    ///< Day of week (0-6, 0=Sunday)
    uint8_t date;       ///< Date (1-31)
    uint8_t month;      ///< Month (1-12)
    uint16_t year;      ///< Year (2000-2099)
} rtc_time_t;

/**
 * @brief RTC Configuration Structure
 */
typedef struct {
    i2c_port_t i2c_port;        ///< I2C port number
    gpio_num_t sda_pin;         ///< I2C SDA pin
    gpio_num_t scl_pin;         ///< I2C SCL pin
    gpio_num_t int_pin;         ///< RTC interrupt pin
    uint32_t i2c_freq_hz;       ///< I2C frequency (default: 100000)
} rtc_config_t;

/**
 * @brief Initialize RTC driver
 *
 * @param config RTC configuration
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_rv3028_init(const rtc_config_t *config);

/**
 * @brief Deinitialize RTC driver
 *
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_rv3028_deinit(void);

/**
 * @brief Set RTC time
 * 
 * @param time Time structure to set
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_set_time(const rtc_time_t *time);

/**
 * @brief Get RTC time
 * 
 * @param time Pointer to store time
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_get_time(rtc_time_t *time);

/**
 * @brief Set Unix timestamp
 * 
 * @param timestamp Unix timestamp (seconds since 1970-01-01)
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_set_unix_time(uint32_t timestamp);

/**
 * @brief Get Unix timestamp
 * 
 * @param timestamp Pointer to store timestamp
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_get_unix_time(uint32_t *timestamp);

/**
 * @brief Set alarm for wake-up
 * 
 * @param timestamp Unix timestamp for alarm
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_set_alarm(uint32_t timestamp);

/**
 * @brief Clear alarm interrupt
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_clear_alarm(void);

/**
 * @brief Enable periodic interrupt
 * 
 * @param interval_seconds Interval in seconds
 * @return esp_err_t ESP_OK on success
 */
esp_err_t rtc_enable_periodic_interrupt(uint32_t interval_seconds);

/**
 * @brief Check if RTC is connected and responding
 * 
 * @return true if RTC is available
 */
bool rtc_is_available(void);

#ifdef __cplusplus
}
#endif

#endif // RTC_RV3028_H

