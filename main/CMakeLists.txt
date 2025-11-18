idf_component_register(
    SRCS
        "main.c"
    INCLUDE_DIRS
        "."
    REQUIRES
        driver
        esp_wifi
        nvs_flash
        esp_timer
        esp_pm
        rtc_driver
        espnow_sync
        sleep_manager
)

