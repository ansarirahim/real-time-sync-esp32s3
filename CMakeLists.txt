# ESP-NOW RTC Synchronization Project
# For ESP32-S3 Gateway and Sensor Nodes
# Minimum ESP-IDF version: v5.0

cmake_minimum_required(VERSION 3.16)

# Project version
set(PROJECT_VER "1.0.0")
set(PROJECT_VER_MILESTONE "M1")

# Include ESP-IDF build system
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Project name
project(esp32_rtc_sync)

