# ESP-IDF Setup Guide for Windows

**Developer:** Abdul Raheem Ansari  
**Email:** ansarirahim1@gmail.com | **WhatsApp:** +91 9024304883  
**LinkedIn:** [linkedin.com/in/abdul-raheem-ansari-a6871320](https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/)

---

## Quick Setup (Recommended)

### Option 1: ESP-IDF Installer (Easiest)

1. **Download ESP-IDF Installer:**
   - Go to: https://dl.espressif.com/dl/esp-idf/
   - Download: `esp-idf-tools-setup-online-5.3.exe` (or latest version)
   - **Direct Link:** https://dl.espressif.com/dl/esp-idf/

2. **Run Installer:**
   - Double-click the installer
   - Select "ESP-IDF v5.3" (or latest stable)
   - Choose installation directory (default: `C:\Espressif`)
   - Install all components (Python, Git, CMake, Ninja, etc.)
   - Wait for installation (10-20 minutes)

3. **Verify Installation:**
   - Open "ESP-IDF 5.3 CMD" from Start Menu
   - Run: `idf.py --version`
   - Should show: `ESP-IDF v5.3` (or your version)

4. **Navigate to Project:**
   ```cmd
   cd C:\Users\Abdul\Documents\augment-projects\ridvan
   ```

5. **Build Project:**
   ```cmd
   idf.py set-target esp32s3
   idf.py build
   ```

---

## Option 2: Manual Setup (Advanced)

### Prerequisites

- Python 3.8+ (✓ You have Python 3.11.9)
- Git
- CMake
- Ninja build system

### Installation Steps

1. **Install Git (if not installed):**
   ```powershell
   winget install Git.Git
   ```

2. **Clone ESP-IDF:**
   ```powershell
   cd C:\
   mkdir Espressif
   cd Espressif
   git clone --recursive https://github.com/espressif/esp-idf.git
   cd esp-idf
   git checkout v5.3
   git submodule update --init --recursive
   ```

3. **Install ESP-IDF Tools:**
   ```powershell
   .\install.bat esp32s3
   ```

4. **Set Up Environment:**
   ```powershell
   .\export.bat
   ```

5. **Verify:**
   ```powershell
   idf.py --version
   ```

---

## Building the Project

### First Time Build

1. **Open ESP-IDF Command Prompt:**
   - Start Menu → "ESP-IDF 5.3 CMD"
   - OR run `C:\Espressif\esp-idf\export.bat` in PowerShell

2. **Navigate to Project:**
   ```cmd
   cd C:\Users\Abdul\Documents\augment-projects\ridvan
   ```

3. **Set Target:**
   ```cmd
   idf.py set-target esp32s3
   ```

4. **Build:**
   ```cmd
   idf.py build
   ```

5. **Flash to Board:**
   ```cmd
   idf.py -p COM5 flash
   ```

6. **Monitor Output:**
   ```cmd
   idf.py -p COM5 monitor
   ```

7. **Flash and Monitor (Combined):**
   ```cmd
   idf.py -p COM5 flash monitor
   ```

---

## Troubleshooting

### Issue 1: `idf.py` not recognized

**Solution:**
- Make sure you're using "ESP-IDF CMD" from Start Menu
- OR run `C:\Espressif\esp-idf\export.bat` first

### Issue 2: Python version mismatch

**Solution:**
- ESP-IDF installer includes its own Python
- Use the ESP-IDF CMD prompt (not regular PowerShell)

### Issue 3: COM port not found

**Solution:**
- Check Device Manager for correct COM port
- Your board is on COM5 (verified)
- Install USB-Serial drivers if needed

### Issue 4: Permission denied on COM5

**Solution:**
- Close any serial monitor programs (Arduino IDE, PuTTY, etc.)
- Unplug and replug the USB cable
- Try a different USB port

---

## Quick Reference

### Common Commands

```bash
# Set target (first time only)
idf.py set-target esp32s3

# Build project
idf.py build

# Flash to board
idf.py -p COM5 flash

# Monitor serial output
idf.py -p COM5 monitor

# Flash and monitor
idf.py -p COM5 flash monitor

# Clean build
idf.py fullclean

# Configure project (menuconfig)
idf.py menuconfig

# Erase flash
idf.py -p COM5 erase-flash
```

### Keyboard Shortcuts (Monitor)

- `Ctrl+]` - Exit monitor
- `Ctrl+T` `Ctrl+H` - Show help
- `Ctrl+T` `Ctrl+R` - Reset chip
- `Ctrl+T` `Ctrl+F` - Toggle filter

---

## Expected Build Output

### Successful Build

```
Project build complete. To flash, run:
 idf.py -p COM5 flash
or
 idf.py -p (PORT) flash
or
 python -m esptool --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 8MB --flash_freq 80m 0x0 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition_table.bin 0x10000 build/esp32_rtc_sync.bin
```

### Expected Serial Output

```
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT)
...
I (xxx) MAIN: ===========================================
I (xxx) MAIN: ESP-NOW RTC Synchronization Project
I (xxx) MAIN: Version: 1.0.0-M1 (Milestone 1)
I (xxx) MAIN: Target: ESP32-S3
I (xxx) MAIN: ===========================================
I (xxx) MAIN: NVS initialized
I (xxx) MAIN: Chip: esp32s3 with 2 CPU cores, WiFi/BLE
I (xxx) MAIN: Silicon revision: 2
I (xxx) MAIN: Flash: 8MB embedded
I (xxx) MAIN: Free heap: XXXXXX bytes
```

---

## Next Steps After Setup

1. **Build and flash current firmware** (verify toolchain works)
2. **Check serial output** (verify board is working)
3. **Start Milestone 2 development** (RTC driver, ESP-NOW)
4. **Order RTC modules** (Adafruit or SparkFun)

---

## Resources

- **ESP-IDF Documentation:** https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/
- **ESP-IDF GitHub:** https://github.com/espressif/esp-idf
- **ESP32-S3 Datasheet:** https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- **ESP-NOW Guide:** https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/network/esp_now.html

---

**Document Status:** Complete  
**Next Action:** Install ESP-IDF and build project

