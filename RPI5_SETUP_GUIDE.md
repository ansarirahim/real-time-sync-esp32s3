# Raspberry Pi 5 Development Environment Setup Guide

**Author:** A.R. Ansari  
**Email:** ansarirahim1@gmail.com  
**Date:** November 27, 2024  
**Project:** Ridvan ESP32-S3 RTC Sync System

---

## Overview

This guide will help you set up the complete ESP32-S3 development environment on your Raspberry Pi 5.

---

## Current Status

✅ **Project Transferred:** All files successfully copied to RPI5  
✅ **Location:** `/home/ar/ridvan-esp32-project`  
✅ **System:** Raspberry Pi 5 (aarch64) running Debian Linux  

---

## Setup Steps

### Step 1: Transfer Setup Script to RPI5

From your Windows machine, run:

```bash
scp setup_rpi_esp_idf.sh ar@192.168.1.42:~/
```

Password: `ansari`

### Step 2: SSH into Raspberry Pi 5

```bash
ssh ar@192.168.1.42
```

Password: `ansari`

### Step 3: Run the Setup Script

```bash
cd ~
chmod +x setup_rpi_esp_idf.sh
./setup_rpi_esp_idf.sh
```

**Note:** This will take 15-30 minutes depending on internet speed. The script will:
- Update system packages
- Install all ESP-IDF dependencies
- Clone ESP-IDF v5.5.1
- Install ESP32-S3 toolchain
- Configure Git with your credentials
- Set up USB permissions

### Step 4: Reboot (Important!)

After the script completes, reboot to apply USB permissions:

```bash
sudo reboot
```

### Step 5: Activate ESP-IDF Environment

After reboot, SSH back in and activate ESP-IDF:

```bash
ssh ar@192.168.1.42
. ~/esp/esp-idf/export.sh
```

Or use the alias:

```bash
get_idf
```

### Step 6: Test Build System

```bash
cd ~/ridvan-esp32-project
idf.py set-target esp32s3
idf.py build
```

Expected output: Successful build with binary size ~750 KB

---

## Connecting ESP32-S3 to Raspberry Pi 5

### Hardware Connection

1. Connect ESP32-S3 to RPI5 via USB cable
2. Check if device is detected:

```bash
ls -l /dev/ttyUSB*
```

Expected output: `/dev/ttyUSB0` or `/dev/ttyACM0`

### Flashing Firmware

```bash
cd ~/ridvan-esp32-project
. ~/esp/esp-idf/export.sh
idf.py -p /dev/ttyUSB0 flash
```

### Monitoring Serial Output

```bash
idf.py -p /dev/ttyUSB0 monitor
```

To exit monitor: Press `Ctrl+]`

### Flash and Monitor in One Command

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

---

## Daily Development Workflow

### 1. Start Development Session

```bash
ssh ar@192.168.1.42
cd ~/ridvan-esp32-project
. ~/esp/esp-idf/export.sh
```

### 2. Make Code Changes

Edit files using your preferred editor:
- `nano main/main.c`
- `vim components/rtc_driver/rtc_rv3028.c`
- Or use VS Code Remote SSH extension

### 3. Build Firmware

```bash
idf.py build
```

### 4. Flash to ESP32-S3

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

### 5. Commit Changes

```bash
git add .
git commit -m "Your commit message"
git push
```

---

## Troubleshooting

### Issue: Permission Denied on /dev/ttyUSB0

**Solution:**
```bash
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
sudo reboot
```

### Issue: ESP-IDF Not Found

**Solution:**
```bash
. ~/esp/esp-idf/export.sh
```

Add to `~/.bashrc` for automatic activation:
```bash
echo "alias get_idf='. ~/esp/esp-idf/export.sh'" >> ~/.bashrc
source ~/.bashrc
```

### Issue: Build Fails with Missing Dependencies

**Solution:**
```bash
cd ~/esp/esp-idf
./install.sh esp32s3
```

### Issue: Cannot Connect to ESP32-S3

**Solution:**
1. Check USB connection
2. Try different USB port
3. Check device detection: `dmesg | tail`
4. Try `/dev/ttyACM0` instead of `/dev/ttyUSB0`

---

## Project Structure on RPI5

```
/home/ar/ridvan-esp32-project/
├── components/
│   ├── rtc_driver/          # RTC driver
│   ├── espnow_sync/         # ESP-NOW protocol
│   └── sleep_manager/       # Sleep management
├── main/                    # Main application
├── build/                   # Build artifacts
├── docs/                    # Documentation
└── [Configuration files]
```

---

## Next Steps After Setup

1. ✅ Complete ESP-IDF installation
2. ✅ Test build system
3. ✅ Flash firmware to ESP32-S3
4. ✅ Verify serial monitor output
5. 📋 Submit Milestone 2 to Ridvan
6. 📋 Wait for M2 approval
7. 📋 Discuss M3 requirements with Ridvan
8. 📋 Procure RTC hardware modules
9. 📋 Complete M3 field testing

---

## Contact Information

**Developer:** A.R. Ansari  
**Email:** ansarirahim1@gmail.com  
**WhatsApp:** +919024304833  
**LinkedIn:** https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/

---

**All future development will be done on Raspberry Pi 5!** 🚀

