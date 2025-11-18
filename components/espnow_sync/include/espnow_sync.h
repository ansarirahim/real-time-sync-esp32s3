/**
 * @file espnow_sync.h
 * @brief ESP-NOW Time Synchronization Protocol
 * 
 * @author Abdul Raheem Ansari
 * @email ansarirahim1@gmail.com
 * @date 2024-11-18
 * 
 * This component implements time synchronization between gateway and sensor nodes
 * using ESP-NOW protocol with RTC-based absolute time scheduling.
 */

#ifndef ESPNOW_SYNC_H
#define ESPNOW_SYNC_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_now.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ESP-NOW packet types
 */
typedef enum {
    ESPNOW_PKT_TIME_SYNC = 0x01,    ///< Time synchronization packet
    ESPNOW_PKT_SENSOR_DATA = 0x02,  ///< Sensor data packet
    ESPNOW_PKT_ACK = 0x03,          ///< Acknowledgment packet
    ESPNOW_PKT_WAKE_SCHEDULE = 0x04 ///< Wake schedule update
} espnow_packet_type_t;

/**
 * @brief Device role
 */
typedef enum {
    ESPNOW_ROLE_GATEWAY = 0,    ///< Gateway device (receiver)
    ESPNOW_ROLE_SENSOR = 1      ///< Sensor node (transmitter)
} espnow_role_t;

/**
 * @brief Time sync packet structure
 */
typedef struct __attribute__((packed)) {
    uint8_t type;               ///< Packet type (ESPNOW_PKT_TIME_SYNC)
    uint32_t timestamp;         ///< Current Unix timestamp
    uint32_t next_wake_time;    ///< Next scheduled wake time
    uint16_t wake_interval;     ///< Wake interval in seconds
    uint8_t sequence;           ///< Sequence number
    uint8_t checksum;           ///< Simple checksum
} espnow_time_sync_t;

/**
 * @brief Sensor data packet structure
 */
typedef struct __attribute__((packed)) {
    uint8_t type;               ///< Packet type (ESPNOW_PKT_SENSOR_DATA)
    uint8_t sensor_id;          ///< Sensor node ID
    uint32_t timestamp;         ///< Measurement timestamp
    uint16_t data_count;        ///< Number of data points
    uint8_t sequence;           ///< Sequence number
    uint8_t data[200];          ///< Sensor data payload
    uint8_t checksum;           ///< Simple checksum
} espnow_sensor_data_t;

/**
 * @brief ACK packet structure
 */
typedef struct __attribute__((packed)) {
    uint8_t type;               ///< Packet type (ESPNOW_PKT_ACK)
    uint8_t ack_sequence;       ///< Sequence number being acknowledged
    uint32_t timestamp;         ///< Gateway timestamp
    uint8_t checksum;           ///< Simple checksum
} espnow_ack_t;

/**
 * @brief ESP-NOW configuration
 */
typedef struct {
    espnow_role_t role;         ///< Device role (gateway or sensor)
    uint8_t channel;            ///< WiFi channel (1-13)
    uint8_t sensor_id;          ///< Sensor ID (for sensor nodes)
    uint8_t gateway_mac[6];     ///< Gateway MAC address
    uint16_t wake_interval;     ///< Wake interval in seconds
} espnow_config_t;

/**
 * @brief Callback for received time sync
 * 
 * @param sync_data Time sync packet data
 */
typedef void (*espnow_time_sync_cb_t)(const espnow_time_sync_t *sync_data);

/**
 * @brief Callback for received sensor data
 * 
 * @param sensor_data Sensor data packet
 */
typedef void (*espnow_sensor_data_cb_t)(const espnow_sensor_data_t *sensor_data);

/**
 * @brief Initialize ESP-NOW sync component
 * 
 * @param config ESP-NOW configuration
 * @return esp_err_t ESP_OK on success
 */
esp_err_t espnow_sync_init(const espnow_config_t *config);

/**
 * @brief Deinitialize ESP-NOW sync component
 * 
 * @return esp_err_t ESP_OK on success
 */
esp_err_t espnow_sync_deinit(void);

/**
 * @brief Send time sync packet (gateway only)
 * 
 * @param timestamp Current Unix timestamp
 * @param next_wake_time Next scheduled wake time
 * @return esp_err_t ESP_OK on success
 */
esp_err_t espnow_send_time_sync(uint32_t timestamp, uint32_t next_wake_time);

/**
 * @brief Send sensor data packet (sensor only)
 * 
 * @param data Sensor data buffer
 * @param data_len Length of data
 * @param timestamp Measurement timestamp
 * @return esp_err_t ESP_OK on success
 */
esp_err_t espnow_send_sensor_data(const uint8_t *data, uint16_t data_len, uint32_t timestamp);

/**
 * @brief Register time sync callback
 * 
 * @param callback Callback function
 */
void espnow_register_time_sync_cb(espnow_time_sync_cb_t callback);

/**
 * @brief Register sensor data callback
 * 
 * @param callback Callback function
 */
void espnow_register_sensor_data_cb(espnow_sensor_data_cb_t callback);

/**
 * @brief Get last received timestamp
 * 
 * @return uint32_t Last timestamp
 */
uint32_t espnow_get_last_timestamp(void);

/**
 * @brief Check if time is synchronized
 * 
 * @return true if synchronized
 */
bool espnow_is_time_synced(void);

#ifdef __cplusplus
}
#endif

#endif // ESPNOW_SYNC_H

