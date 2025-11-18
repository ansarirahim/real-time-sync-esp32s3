idf_component_register(SRCS "espnow_sync.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi esp_netif nvs_flash)

