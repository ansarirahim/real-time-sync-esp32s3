#!/bin/bash
################################################################################
# ESP-IDF Setup Script for Raspberry Pi 5
# Author: A.R. Ansari
# Date: November 27, 2024
# Purpose: Install and configure ESP-IDF v5.5.1 on Raspberry Pi 5
################################################################################

set -e  # Exit on error

echo "========================================="
echo "ESP-IDF Setup for Raspberry Pi 5"
echo "========================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
ESP_IDF_VERSION="v5.5.1"
ESP_IDF_PATH="$HOME/esp/esp-idf"
PROJECT_PATH="$HOME/ridvan-esp32-project"

echo -e "${YELLOW}Step 1: Updating system packages...${NC}"
sudo apt update
sudo apt upgrade -y

echo ""
echo -e "${YELLOW}Step 2: Installing ESP-IDF dependencies...${NC}"
sudo apt install -y \
    git \
    wget \
    flex \
    bison \
    gperf \
    python3 \
    python3-pip \
    python3-venv \
    cmake \
    ninja-build \
    ccache \
    libffi-dev \
    libssl-dev \
    dfu-util \
    libusb-1.0-0 \
    python3-setuptools

echo ""
echo -e "${YELLOW}Step 3: Creating ESP directory...${NC}"
mkdir -p $HOME/esp
cd $HOME/esp

echo ""
echo -e "${YELLOW}Step 4: Cloning ESP-IDF repository (this may take a while)...${NC}"
if [ -d "$ESP_IDF_PATH" ]; then
    echo -e "${GREEN}ESP-IDF already exists, updating...${NC}"
    cd $ESP_IDF_PATH
    git fetch
    git checkout $ESP_IDF_VERSION
    git submodule update --init --recursive
else
    git clone --recursive https://github.com/espressif/esp-idf.git
    cd esp-idf
    git checkout $ESP_IDF_VERSION
    git submodule update --init --recursive
fi

echo ""
echo -e "${YELLOW}Step 5: Installing ESP-IDF tools for ESP32-S3...${NC}"
cd $ESP_IDF_PATH
./install.sh esp32s3

echo ""
echo -e "${YELLOW}Step 6: Configuring Git...${NC}"
git config --global user.email "ansarirahim1@gmail.com"
git config --global user.name "A.R. Ansari"

echo ""
echo -e "${YELLOW}Step 7: Adding ESP-IDF to shell profile...${NC}"
SHELL_PROFILE="$HOME/.bashrc"
if ! grep -q "export IDF_PATH=" "$SHELL_PROFILE"; then
    echo "" >> "$SHELL_PROFILE"
    echo "# ESP-IDF Configuration" >> "$SHELL_PROFILE"
    echo "export IDF_PATH=$ESP_IDF_PATH" >> "$SHELL_PROFILE"
    echo "alias get_idf='. \$IDF_PATH/export.sh'" >> "$SHELL_PROFILE"
    echo -e "${GREEN}Added ESP-IDF configuration to $SHELL_PROFILE${NC}"
else
    echo -e "${GREEN}ESP-IDF already configured in $SHELL_PROFILE${NC}"
fi

echo ""
echo -e "${YELLOW}Step 8: Setting up USB permissions for ESP32 flashing...${NC}"
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER

echo ""
echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}ESP-IDF Setup Complete!${NC}"
echo -e "${GREEN}=========================================${NC}"
echo ""
echo -e "${YELLOW}IMPORTANT: Next Steps${NC}"
echo ""
echo "1. Log out and log back in (or reboot) for USB permissions to take effect"
echo ""
echo "2. To activate ESP-IDF in your current terminal, run:"
echo -e "   ${GREEN}. $ESP_IDF_PATH/export.sh${NC}"
echo ""
echo "3. Or use the alias (after reloading shell):"
echo -e "   ${GREEN}get_idf${NC}"
echo ""
echo "4. To test the build system, run:"
echo -e "   ${GREEN}cd $PROJECT_PATH${NC}"
echo -e "   ${GREEN}. $ESP_IDF_PATH/export.sh${NC}"
echo -e "   ${GREEN}idf.py set-target esp32s3${NC}"
echo -e "   ${GREEN}idf.py build${NC}"
echo ""
echo "5. To flash to ESP32-S3 (after connecting via USB):"
echo -e "   ${GREEN}idf.py -p /dev/ttyUSB0 flash monitor${NC}"
echo ""
echo -e "${YELLOW}Installation Details:${NC}"
echo "  ESP-IDF Version: $ESP_IDF_VERSION"
echo "  ESP-IDF Path: $ESP_IDF_PATH"
echo "  Project Path: $PROJECT_PATH"
echo "  Target: ESP32-S3"
echo ""
echo -e "${GREEN}Setup script completed successfully!${NC}"

