/**
 * @file espnow_sync.c
 * @brief ESP-NOW Time Synchronization Implementation
 * 
 * @author Abdul Raheem Ansari
 * @email ansarirahim1@gmail.com
 * @date 2024-11-18
 */

#include "espnow_sync.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_log.h"
#include "esp_crc.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include <string.h>

static const char *TAG = "ESPNOW_SYNC";

// MAC address format string for ESP-IDF v5.5+
#define MACSTR_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR_ARG(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]

static espnow_config_t espnow_config;
static bool espnow_initialized = false;
static uint8_t sequence_number = 0;
static uint32_t last_timestamp = 0;
static bool time_synced = false;

static espnow_time_sync_cb_t time_sync_callback = NULL;
static espnow_sensor_data_cb_t sensor_data_callback = NULL;

/**
 * @brief Calculate simple checksum
 */
static uint8_t calculate_checksum(const uint8_t *data, size_t len) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < len; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

/**
 * @brief ESP-NOW receive callback
 */
static void espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    if (data == NULL || len < 1) {
        ESP_LOGW(TAG, "Received invalid packet");
        return;
    }

    uint8_t packet_type = data[0];

    ESP_LOGI(TAG, "Received packet type: 0x%02X, len: %d from " MACSTR_FMT,
             packet_type, len, MAC2STR_ARG(recv_info->src_addr));

    switch (packet_type) {
        case ESPNOW_PKT_TIME_SYNC: {
            if (len == sizeof(espnow_time_sync_t)) {
                espnow_time_sync_t *sync = (espnow_time_sync_t *)data;
                
                // Verify checksum
                uint8_t calc_checksum = calculate_checksum(data, len - 1);
                if (calc_checksum != sync->checksum) {
                    ESP_LOGW(TAG, "Time sync checksum mismatch");
                    break;
                }
                
                last_timestamp = sync->timestamp;
                time_synced = true;
                
                ESP_LOGI(TAG, "Time sync received: timestamp=%lu, next_wake=%lu",
                         (unsigned long)sync->timestamp, 
                         (unsigned long)sync->next_wake_time);
                
                if (time_sync_callback) {
                    time_sync_callback(sync);
                }
            }
            break;
        }
        
        case ESPNOW_PKT_SENSOR_DATA: {
            if (len == sizeof(espnow_sensor_data_t)) {
                espnow_sensor_data_t *sensor = (espnow_sensor_data_t *)data;
                
                // Verify checksum
                uint8_t calc_checksum = calculate_checksum(data, len - 1);
                if (calc_checksum != sensor->checksum) {
                    ESP_LOGW(TAG, "Sensor data checksum mismatch");
                    break;
                }
                
                ESP_LOGI(TAG, "Sensor data received: id=%d, count=%d, timestamp=%lu",
                         sensor->sensor_id, sensor->data_count,
                         (unsigned long)sensor->timestamp);
                
                if (sensor_data_callback) {
                    sensor_data_callback(sensor);
                }
                
                // Send ACK if we're the gateway
                if (espnow_config.role == ESPNOW_ROLE_GATEWAY) {
                    espnow_ack_t ack = {
                        .type = ESPNOW_PKT_ACK,
                        .ack_sequence = sensor->sequence,
                        .timestamp = last_timestamp
                    };
                    ack.checksum = calculate_checksum((uint8_t *)&ack, sizeof(ack) - 1);
                    
                    esp_now_send(recv_info->src_addr, (uint8_t *)&ack, sizeof(ack));
                    ESP_LOGI(TAG, "ACK sent for sequence %d", sensor->sequence);
                }
            }
            break;
        }
        
        case ESPNOW_PKT_ACK: {
            if (len == sizeof(espnow_ack_t)) {
                espnow_ack_t *ack = (espnow_ack_t *)data;
                ESP_LOGI(TAG, "ACK received for sequence %d", ack->ack_sequence);
            }
            break;
        }
        
        default:
            ESP_LOGW(TAG, "Unknown packet type: 0x%02X", packet_type);
            break;
    }
}

/**
 * @brief ESP-NOW send callback (ESP-IDF v5.5+ signature)
 */
static void espnow_send_cb(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
    ESP_LOGI(TAG, "Send to " MACSTR_FMT " %s",
             MAC2STR_ARG(tx_info->des_addr),
             status == ESP_NOW_SEND_SUCCESS ? "SUCCESS" : "FAILED");
}

esp_err_t espnow_sync_init(const espnow_config_t *config) {
    if (config == NULL) {
        ESP_LOGE(TAG, "Config is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Initializing ESP-NOW sync component");
    ESP_LOGI(TAG, "Role: %s, Channel: %d",
             config->role == ESPNOW_ROLE_GATEWAY ? "GATEWAY" : "SENSOR",
             config->channel);

    memcpy(&espnow_config, config, sizeof(espnow_config_t));

    // Initialize WiFi
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(config->channel, WIFI_SECOND_CHAN_NONE));

    ESP_LOGI(TAG, "WiFi initialized on channel %d", config->channel);

    // Initialize ESP-NOW
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_cb));
    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_cb));

    ESP_LOGI(TAG, "ESP-NOW initialized");

    // Add peer (gateway or broadcast)
    esp_now_peer_info_t peer;
    memset(&peer, 0, sizeof(esp_now_peer_info_t));
    peer.channel = config->channel;
    peer.ifidx = WIFI_IF_STA;
    peer.encrypt = false;

    if (config->role == ESPNOW_ROLE_SENSOR) {
        // Sensor: add gateway as peer
        memcpy(peer.peer_addr, config->gateway_mac, 6);
        ESP_LOGI(TAG, "Adding gateway peer: " MACSTR_FMT, MAC2STR_ARG(config->gateway_mac));
    } else {
        // Gateway: add broadcast address for receiving from all sensors
        memset(peer.peer_addr, 0xFF, 6);
        ESP_LOGI(TAG, "Adding broadcast peer for gateway");
    }

    ESP_ERROR_CHECK(esp_now_add_peer(&peer));

    espnow_initialized = true;
    ESP_LOGI(TAG, "ESP-NOW sync component initialized successfully");

    return ESP_OK;
}

esp_err_t espnow_sync_deinit(void) {
    if (!espnow_initialized) {
        return ESP_OK;
    }

    esp_now_deinit();
    esp_wifi_stop();
    esp_wifi_deinit();

    espnow_initialized = false;
    ESP_LOGI(TAG, "ESP-NOW sync component deinitialized");

    return ESP_OK;
}

esp_err_t espnow_send_time_sync(uint32_t timestamp, uint32_t next_wake_time) {
    if (!espnow_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    if (espnow_config.role != ESPNOW_ROLE_GATEWAY) {
        ESP_LOGE(TAG, "Only gateway can send time sync");
        return ESP_ERR_INVALID_STATE;
    }

    espnow_time_sync_t sync = {
        .type = ESPNOW_PKT_TIME_SYNC,
        .timestamp = timestamp,
        .next_wake_time = next_wake_time,
        .wake_interval = espnow_config.wake_interval,
        .sequence = sequence_number++
    };

    sync.checksum = calculate_checksum((uint8_t *)&sync, sizeof(sync) - 1);

    // Broadcast to all sensors
    uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_err_t ret = esp_now_send(broadcast_mac, (uint8_t *)&sync, sizeof(sync));

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Time sync sent: timestamp=%lu, next_wake=%lu",
                 (unsigned long)timestamp, (unsigned long)next_wake_time);
    } else {
        ESP_LOGE(TAG, "Failed to send time sync: %s", esp_err_to_name(ret));
    }

    return ret;
}

esp_err_t espnow_send_sensor_data(const uint8_t *data, uint16_t data_len, uint32_t timestamp) {
    if (!espnow_initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    if (espnow_config.role != ESPNOW_ROLE_SENSOR) {
        ESP_LOGE(TAG, "Only sensor can send sensor data");
        return ESP_ERR_INVALID_STATE;
    }

    if (data_len > sizeof(((espnow_sensor_data_t *)0)->data)) {
        ESP_LOGE(TAG, "Data too large: %d bytes", data_len);
        return ESP_ERR_INVALID_SIZE;
    }

    espnow_sensor_data_t sensor_pkt = {
        .type = ESPNOW_PKT_SENSOR_DATA,
        .sensor_id = espnow_config.sensor_id,
        .timestamp = timestamp,
        .data_count = data_len,
        .sequence = sequence_number++
    };

    memcpy(sensor_pkt.data, data, data_len);
    sensor_pkt.checksum = calculate_checksum((uint8_t *)&sensor_pkt, sizeof(sensor_pkt) - 1);

    esp_err_t ret = esp_now_send(espnow_config.gateway_mac, (uint8_t *)&sensor_pkt, sizeof(sensor_pkt));

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Sensor data sent: %d bytes, timestamp=%lu",
                 data_len, (unsigned long)timestamp);
    } else {
        ESP_LOGE(TAG, "Failed to send sensor data: %s", esp_err_to_name(ret));
    }

    return ret;
}

void espnow_register_time_sync_cb(espnow_time_sync_cb_t callback) {
    time_sync_callback = callback;
    ESP_LOGI(TAG, "Time sync callback registered");
}

void espnow_register_sensor_data_cb(espnow_sensor_data_cb_t callback) {
    sensor_data_callback = callback;
    ESP_LOGI(TAG, "Sensor data callback registered");
}

uint32_t espnow_get_last_timestamp(void) {
    return last_timestamp;
}

bool espnow_is_time_synced(void) {
    return time_synced;
}

