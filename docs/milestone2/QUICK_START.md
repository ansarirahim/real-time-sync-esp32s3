# Quick Start Guide - Milestone 2

**Developer:** Abdul Raheem Ansari  
**Email:** ansarirahim1@gmail.com | **WhatsApp:** +91 9024304883

---

## 🚀 Get Started in 5 Steps

### Step 1: Install ESP-IDF (20 minutes)

**Download installer:**
👉 https://dl.espressif.com/dl/esp-idf/

**Run:** `esp-idf-tools-setup-online-5.3.exe`

**Select:** ESP-IDF v5.3, install to `C:\Espressif`

**Verify:** Open "ESP-IDF 5.3 CMD" from Start Menu

---

### Step 2: Build the Firmware (5 minutes)

Open **ESP-IDF 5.3 CMD** and run:

```bash
cd C:\Users\Abdul\Documents\augment-projects\ridvan
idf.py set-target esp32s3
idf.py build
```

**Expected:** Build completes successfully

---

### Step 3: Flash to ESP32-S3 (2 minutes)

```bash
idf.py -p COM5 flash monitor
```

**Expected:** Firmware uploads and serial monitor shows:
```
I (xxx) MAIN: ESP-NOW RTC Synchronization Project
I (xxx) MAIN: Version: 1.0.0-M2 (Milestone 2)
I (xxx) MAIN: Role: SENSOR
I (xxx) RTC_RV3028: RTC not detected - will use stub mode
I (xxx) ESPNOW_SYNC: ESP-NOW initialized
I (xxx) SLEEP_MGR: Sleep manager initialized
I (xxx) MAIN: All components initialized successfully!
```

**To exit monitor:** Press `Ctrl + ]`

---

### Step 4: Configure Device Role

Edit `main/main.c` line 32:

**For Gateway:**
```c
#define DEVICE_ROLE  ESPNOW_ROLE_GATEWAY
```

**For Sensor:**
```c
#define DEVICE_ROLE  ESPNOW_ROLE_SENSOR
#define SENSOR_ID    1  // Change to 2 or 3 for other sensors
```

**Rebuild and flash:**
```bash
idf.py build flash monitor
```

---

### Step 5: Test Communication

**Setup:**
1. Flash one ESP32-S3 as **GATEWAY**
2. Flash another ESP32-S3 as **SENSOR** (ID=1)
3. Update gateway MAC address in sensor code (line 45)

**Get Gateway MAC:**
```
I (xxx) ESPNOW_SYNC: WiFi initialized on channel 1
```
Look for MAC address in logs.

**Update Sensor Code:**
```c
static uint8_t gateway_mac[6] = {0x98, 0xa3, 0x16, 0xf0, 0x6c, 0x64};
```

**Expected Behavior:**
- Gateway broadcasts time sync every 60 seconds
- Sensor receives time sync
- Sensor sends data to gateway
- Gateway receives data and sends ACK
- Sensor enters deep sleep

---

## 📊 What You'll See

### Gateway Logs:
```
I (xxx) MAIN: Gateway mode: Listening for sensor data...
I (xxx) ESPNOW_SYNC: Time sync sent: timestamp=1700000000
I (xxx) ESPNOW_SYNC: Sensor data received: id=1, count=10
I (xxx) ESPNOW_SYNC: ACK sent for sequence 0
```

### Sensor Logs:
```
I (xxx) MAIN: Sensor mode: Wake -> Measure -> Transmit -> Sleep
I (xxx) MAIN: Waiting for time sync from gateway...
I (xxx) ESPNOW_SYNC: Time sync received: timestamp=1700000000
I (xxx) MAIN: Time synchronized!
I (xxx) ESPNOW_SYNC: Sensor data sent: 10 bytes
I (xxx) MAIN: Entering deep sleep for 3600 seconds...
```

---

## 🔧 Troubleshooting

### Build Fails

**Error:** `idf.py: command not found`  
**Solution:** Use "ESP-IDF 5.3 CMD" from Start Menu, not regular PowerShell

**Error:** `CMake Error`  
**Solution:** Delete `build/` folder and rebuild:
```bash
rm -r build
idf.py build
```

### Flash Fails

**Error:** `Failed to connect to ESP32`  
**Solution:** 
- Check USB cable
- Try different USB port
- Press and hold BOOT button while flashing

**Error:** `COM5 not found`  
**Solution:** Check Device Manager, install CH340/CP2102 drivers

### ESP-NOW Not Working

**Issue:** Sensor doesn't receive time sync  
**Solution:**
- Verify both devices on same WiFi channel (line 35)
- Update gateway MAC address in sensor code
- Check both devices are powered on

**Issue:** Gateway doesn't receive sensor data  
**Solution:**
- Verify gateway MAC in sensor matches actual gateway MAC
- Check serial monitor for "Send to XX:XX:XX:XX:XX:XX SUCCESS"

---

## 🎯 Next Steps

### With RTC Hardware:

1. **Order RTC modules** (3x RV-3028-C7, ~$21)
   - Adafruit: https://www.adafruit.com/product/5188
   - SparkFun: https://www.sparkfun.com/products/16281

2. **Connect RTC to ESP32-S3:**
   - RTC VCC → ESP32 3.3V
   - RTC GND → ESP32 GND
   - RTC SDA → GPIO8
   - RTC SCL → GPIO9
   - RTC INT → GPIO3

3. **Test RTC:**
   - Flash firmware
   - Check logs: "RTC detected and responding"
   - Set time: `rtc_set_unix_time(1700000000)`
   - Read time: `rtc_get_unix_time(&timestamp)`

4. **Test Deep Sleep Wake:**
   - Set alarm: `rtc_set_alarm(timestamp + 60)`
   - Enter sleep: `sleep_for_seconds(60)`
   - Verify wake from RTC alarm

### Without RTC Hardware:

✅ **You can still test 80% of functionality!**

- ESP-NOW communication works
- Sleep manager works (timer wake)
- All components initialize
- Gateway/sensor communication works
- Only RTC-specific features need hardware

---

## 📞 Need Help?

**Abdul Raheem Ansari**

- **Email:** ansarirahim1@gmail.com
- **WhatsApp:** +91 9024304883
- **LinkedIn:** [linkedin.com/in/abdul-raheem-ansari-a6871320](https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/)

**Take a screenshot of any errors and send them to me!**

---

**Project Repository:** [real-time-sync-esp32s3](https://github.com/ansarirahim/real-time-sync-esp32s3)

*This document is part of the ESP-NOW RTC Synchronization Project for Orbikey Bilisim.*

