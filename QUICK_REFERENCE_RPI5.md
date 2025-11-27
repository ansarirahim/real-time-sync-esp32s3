# Quick Reference - Raspberry Pi 5 Development

**Project:** Ridvan ESP32-S3 RTC Sync System  
**Date:** November 27, 2024

---

## 🚀 IMMEDIATE NEXT STEPS

### 1. SSH into Raspberry Pi 5
```bash
ssh ar@192.168.1.42
```
Password: `ansari`

### 2. Run Setup Script
```bash
chmod +x ~/setup_rpi_esp_idf.sh
~/setup_rpi_esp_idf.sh
```

**Time:** 15-30 minutes  
**What it does:** Installs ESP-IDF v5.5.1 and all dependencies

### 3. Reboot (Important!)
```bash
sudo reboot
```

### 4. Test Build
```bash
ssh ar@192.168.1.42
. ~/esp/esp-idf/export.sh
cd ~/ridvan-esp32-project
idf.py set-target esp32s3
idf.py build
```

---

## 📍 KEY LOCATIONS

| Item | Location |
|------|----------|
| **Project** | `/home/ar/ridvan-esp32-project` |
| **ESP-IDF** | `/home/ar/esp/esp-idf` |
| **Setup Script** | `/home/ar/setup_rpi_esp_idf.sh` |
| **Setup Guide** | `/home/ar/ridvan-esp32-project/RPI5_SETUP_GUIDE.md` |

---

## 🔧 COMMON COMMANDS

### Activate ESP-IDF
```bash
. ~/esp/esp-idf/export.sh
```

### Build Firmware
```bash
cd ~/ridvan-esp32-project
idf.py build
```

### Flash to ESP32-S3
```bash
idf.py -p /dev/ttyUSB0 flash
```

### Monitor Serial Output
```bash
idf.py -p /dev/ttyUSB0 monitor
```

### Flash + Monitor
```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

### Clean Build
```bash
idf.py fullclean
idf.py build
```

---

## 🔌 HARDWARE CONNECTION

1. Connect ESP32-S3 to RPI5 via USB
2. Check device: `ls -l /dev/ttyUSB*`
3. If permission denied: `sudo chmod 666 /dev/ttyUSB0`

---

## 📝 GIT WORKFLOW

### Check Status
```bash
cd ~/ridvan-esp32-project
git status
```

### Commit Changes
```bash
git add .
git commit -m "Your message"
git push
```

### Pull Latest
```bash
git pull
```

---

## 🎯 PROJECT STATUS

✅ **M1:** Hardware Analysis - **APPROVED ($40)**  
✅ **M2:** Firmware Implementation - **READY TO SUBMIT ($70)**  
⏸️ **M3:** Field Testing - **PENDING ($40)**

---

## 📧 SUBMIT M2 TO RIDVAN

**When:** As soon as M2 is activated on Upwork  
**Message:** Use `CLIENT_MESSAGE_M2_ONLY.md`  
**Repository:** https://github.com/ansarirahim/real-time-sync-esp32s3

---

## 🆘 TROUBLESHOOTING

### ESP-IDF Not Found
```bash
. ~/esp/esp-idf/export.sh
```

### Permission Denied on USB
```bash
sudo usermod -a -G dialout $USER
sudo reboot
```

### Build Fails
```bash
cd ~/esp/esp-idf
./install.sh esp32s3
```

---

## 📞 CONTACT

**Developer:** A.R. Ansari  
**Email:** ansarirahim1@gmail.com  
**WhatsApp:** +919024304833

---

## 🎉 SUMMARY

**What's Done:**
- ✅ Project transferred to RPI5 (all 348KB of files)
- ✅ Setup script created and uploaded
- ✅ Setup guide created
- ✅ M2 ready for submission

**What's Next:**
1. Run setup script on RPI5
2. Test build system
3. Submit M2 to Ridvan
4. Wait for M2 approval
5. Discuss M3 with Ridvan

**All future development will be on Raspberry Pi 5!** 🚀

