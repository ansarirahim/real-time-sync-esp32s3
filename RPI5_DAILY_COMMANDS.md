# Raspberry Pi 5 - Daily Development Commands

**Quick reference for ESP32-S3 development on RPI5**

---

## 🔐 SSH Connection

```bash
ssh ar@192.168.1.42
# Password: ansari
```

---

## ⚡ Activate ESP-IDF (REQUIRED EVERY TIME)

```bash
. ~/esp/esp-idf/export.sh
```

**Output:** You'll see ESP-IDF environment variables being set.

---

## 📂 Navigate to Project

```bash
cd ~/ridvan-esp32-project
```

---

## 🔨 Build Commands

### **Set Target (First Time Only)**
```bash
idf.py set-target esp32s3
```

### **Build Firmware**
```bash
idf.py build
```

### **Clean Build**
```bash
idf.py fullclean
idf.py build
```

### **Build Size Analysis**
```bash
idf.py size
idf.py size-components
```

---

## 📡 Flash & Monitor

### **Check Serial Port**
```bash
ls -l /dev/ttyUSB*
# or
ls -l /dev/ttyACM*
```

### **Flash Firmware**
```bash
idf.py -p /dev/ttyUSB0 flash
```

### **Monitor Serial Output**
```bash
idf.py -p /dev/ttyUSB0 monitor
```

### **Flash + Monitor (Combined)**
```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

### **Exit Monitor**
Press: `Ctrl + ]`

---

## 🔧 Configuration

### **Open Menuconfig**
```bash
idf.py menuconfig
```

### **Save Configuration**
```bash
idf.py save-defconfig
```

---

## 📝 Git Commands

### **Check Status**
```bash
git status
```

### **Commit Changes**
```bash
git add .
git commit -m "Your message"
```

### **Push to GitHub**
```bash
git push origin milestone/m2-firmware-implementation
```

### **Pull Latest**
```bash
git pull origin milestone/m2-firmware-implementation
```

---

## 🧹 Cleanup

### **Clean Build Artifacts**
```bash
idf.py fullclean
```

### **Remove Build Directory**
```bash
rm -rf build/
```

---

## 🔍 Debugging

### **Check ESP-IDF Version**
```bash
cd ~/esp/esp-idf
git describe --tags
```

### **Check Python Environment**
```bash
which python
python --version
```

### **Check Toolchain**
```bash
xtensa-esp32s3-elf-gcc --version
```

### **Verify ESP-IDF Installation**
```bash
idf.py --version
```

---

## 📊 Project Info

### **List Components**
```bash
ls -la components/
```

### **View Main Code**
```bash
cat main/main.c
```

### **Check Build Output**
```bash
ls -lh build/*.bin
```

---

## 🚀 Complete Workflow Example

```bash
# 1. SSH into RPI5
ssh ar@192.168.1.42

# 2. Activate ESP-IDF
. ~/esp/esp-idf/export.sh

# 3. Go to project
cd ~/ridvan-esp32-project

# 4. Make code changes (use nano or vi)
nano main/main.c

# 5. Build
idf.py build

# 6. Flash and monitor
idf.py -p /dev/ttyUSB0 flash monitor

# 7. Exit monitor (Ctrl+])

# 8. Commit changes
git add .
git commit -m "Updated sensor logic"
git push

# 9. Exit SSH
exit
```

---

## 💡 Pro Tips

1. **Create alias for ESP-IDF activation:**
   ```bash
   echo 'alias get_idf=". ~/esp/esp-idf/export.sh"' >> ~/.bashrc
   source ~/.bashrc
   # Now you can just type: get_idf
   ```

2. **Set default serial port:**
   ```bash
   export ESPPORT=/dev/ttyUSB0
   # Then you can use: idf.py flash monitor
   ```

3. **Faster builds with ccache:**
   ```bash
   export IDF_CCACHE_ENABLE=1
   ```

4. **Monitor without flashing:**
   ```bash
   idf.py -p /dev/ttyUSB0 monitor
   ```

---

## 🆘 Troubleshooting

### **Permission Denied on Serial Port**
```bash
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
# Then logout and login again
```

### **ESP-IDF Not Found**
```bash
# Make sure you activated it:
. ~/esp/esp-idf/export.sh
```

### **Build Errors**
```bash
# Try clean build:
idf.py fullclean
idf.py build
```

### **Can't Find Serial Port**
```bash
# Check if device is connected:
lsusb
dmesg | tail -20
```

---

**Remember:** Always activate ESP-IDF environment first! ⚡

