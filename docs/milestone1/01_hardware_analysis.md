# Milestone 1: Hardware Analysis Report

## ESP32-S3 Hardware Analysis for RTC Integration

**Project:** ESP-NOW RTC Synchronization System  
**Target Hardware:** ESP32-S3  
**Date:** 2024-11-18  
**Author:** Abdul Raheem Ansari  
**Version:** 1.0.0-M1

---

## 1. ESP32-S3 Overview

### 1.1 Chip Specifications

- **CPU:** Xtensa dual-core 32-bit LX7 microprocessor
- **Clock Frequency:** Up to 240 MHz
- **SRAM:** 512 KB
- **ROM:** 384 KB
- **Flash:** 4 MB (typical development boards)
- **WiFi:** 802.11 b/g/n (2.4 GHz)
- **Bluetooth:** Bluetooth 5 (LE)
- **GPIO Pins:** 45 programmable GPIOs
- **Operating Voltage:** 3.0V - 3.6V (3.3V typical)
- **Operating Temperature:** -40°C to +85°C

### 1.2 Power Consumption

| Mode | Current Consumption | Notes |
|------|---------------------|-------|
| Active (WiFi TX) | ~200 mA | Peak during transmission |
| Active (WiFi RX) | ~100 mA | Receiving data |
| Active (CPU only) | ~40-80 mA | No WiFi |
| Modem sleep | ~20-30 mA | WiFi off, CPU running |
| Light sleep | ~2-3 mA | CPU paused, peripherals off |
| Deep sleep | ~5-10 µA | Only RTC and ULP active |
| Deep sleep (RTC off) | ~2.5 µA | Minimum power mode |

**Note:** With external RTC, we can achieve <100 µA total (ESP32 deep sleep + RTC standby).

---

## 2. GPIO Availability Analysis

### 2.1 Available GPIO Pins for RTC Integration

ESP32-S3 has 45 GPIO pins, but not all are suitable for external peripherals. Here's the analysis:

#### Strapping Pins (Use with caution)
- GPIO0: Boot mode selection
- GPIO3: JTAG enable
- GPIO45: VDD_SPI voltage
- GPIO46: Boot mode selection

#### Reserved/Special Function Pins
- GPIO19-20: USB D-/D+ (if using USB)
- GPIO26-32: SPI flash/PSRAM (typically reserved)
- GPIO33-37: JTAG (if debugging)

#### Recommended GPIO Pins for RTC Integration

**For I2C Communication:**
- **GPIO8 (SDA):** Safe, no conflicts
- **GPIO9 (SCL):** Safe, no conflicts
- Alternative: GPIO1/GPIO2, GPIO4/GPIO5

**For RTC Interrupt (INT):**
- **GPIO3:** Recommended for wake-up (supports ext0 wake source)
- Alternative: GPIO2, GPIO4, GPIO5

**For Button (Gateway Only):**
- **GPIO4:** Recommended for manual wake button
- Supports ext1 wake source

### 2.2 Pin Mapping Summary

| Function | GPIO Pin | Direction | Pull-up/down | Notes |
|----------|----------|-----------|--------------|-------|
| I2C SDA | GPIO8 | Bidirectional | 4.7kΩ external pull-up | I2C data line |
| I2C SCL | GPIO9 | Output | 4.7kΩ external pull-up | I2C clock line |
| RTC INT | GPIO3 | Input | 10kΩ external pull-up | Wake-up source (ext0) |
| Button (Gateway) | GPIO4 | Input | 10kΩ pull-down | Manual wake (ext1) |
| 3.3V Power | 3V3 | Power | - | RTC power supply |
| Ground | GND | Ground | - | Common ground |

---

## 3. Power Supply Analysis

### 3.1 ESP32-S3 Power Requirements

- **Operating Voltage:** 3.0V - 3.6V (3.3V nominal)
- **Recommended Decoupling:** 10µF + 100nF capacitors near VDD pins
- **Peak Current:** Up to 500 mA during WiFi transmission
- **Deep Sleep Current:** 5-10 µA (RTC domain only)

### 3.2 RTC Power Requirements

**RV-3028-C7 Specifications:**
- **Operating Voltage:** 1.1V - 5.5V (3.3V compatible)
- **Timekeeping Current:** 45 nA @ 3.3V
- **I2C Active Current:** ~100 µA (during communication)
- **Backup Battery:** Optional (CR2032, 3V)

**Power Supply Options:**
1. **Shared 3.3V Rail:** RTC powered from same 3.3V as ESP32
   - Simpler design
   - RTC loses time when main power is removed
   
2. **Dedicated Battery Backup:** RTC has separate CR2032 battery
   - RTC maintains time even when ESP32 is powered off
   - Recommended for production systems
   - Battery life: 10+ years (45 nA draw)

**Recommendation:** Use shared 3.3V rail for development, add battery backup for production.

### 3.3 Total System Power Budget

| Component | Deep Sleep | Active (ESP-NOW) | Notes |
|-----------|------------|------------------|-------|
| ESP32-S3 | 5-10 µA | 100-200 mA | Dominant power consumer |
| RV-3028-C7 RTC | 45 nA | 100 µA | Negligible in comparison |
| Pull-up resistors (3x 4.7kΩ) | ~2 µA | ~2 µA | I2C + INT pull-ups |
| **Total** | **<20 µA** | **~150 mA** | Deep sleep dominated by ESP32 |

**Key Insight:** External RTC adds negligible power consumption (<0.05 µA) compared to ESP32 deep sleep current.

---

## 4. I2C Bus Configuration

### 4.1 I2C Specifications

**ESP32-S3 I2C Controller:**
- 2 independent I2C controllers (I2C0, I2C1)
- Master and slave modes supported
- Standard mode: 100 kHz
- Fast mode: 400 kHz
- Fast mode plus: 1 MHz (not recommended for RTC)

**RV-3028-C7 I2C Specifications:**
- I2C address: 0x52 (7-bit addressing)
- Maximum clock frequency: 400 kHz
- I2C standard compliant

**Recommended Configuration:**
- **I2C Speed:** 100 kHz (lower power, more reliable)
- **Pull-up Resistors:** 4.7kΩ (standard for 3.3V, 100 kHz)
- **Bus Capacitance:** <400 pF (short traces, minimal capacitance)

### 4.2 Pull-up Resistor Calculation

For 3.3V I2C bus at 100 kHz:

```
R_min = (V_DD - V_OL) / I_OL = (3.3V - 0.4V) / 3mA ≈ 1kΩ
R_max = t_r / (0.8473 × C_b) = 1000ns / (0.8473 × 200pF) ≈ 5.9kΩ
```

**Recommended:** 4.7kΩ (standard value, well within range)

**Note:** Many RTC breakout boards include pull-up resistors. Check before adding external ones to avoid parallel resistance issues.

---

## 5. Interrupt Configuration

### 5.1 RTC Alarm Interrupt

**RV-3028-C7 INT Pin:**
- Active LOW output
- Open-drain configuration
- Requires external pull-up resistor (10kΩ recommended)
- Triggers on alarm match

**ESP32-S3 Wake-up Configuration:**
- Use `esp_sleep_enable_ext0_wakeup(GPIO_NUM_3, 0)` for RTC INT
- Wake on LOW level (falling edge when alarm triggers)
- INT pin must be pulled HIGH by external resistor

### 5.2 Button Wake-up (Gateway Only)

**Button Configuration:**
- Normally open push button
- Connects 3.3V to GPIO4 when pressed
- 10kΩ pull-down resistor to GND
- Debouncing handled in firmware

**ESP32-S3 Wake-up Configuration:**
- Use `esp_sleep_enable_ext1_wakeup()` for button
- Wake on HIGH level (button pressed)
- Supports multiple GPIO pins (bitmask)

---

## 6. Hardware Compatibility Assessment

### 6.1 ESP32-S3 Development Boards

**Tested/Compatible Boards:**
- ESP32-S3-DevKitC-1 (Espressif official)
- ESP32-S3-DevKitM-1 (Espressif official)
- ESP32-S3-WROOM-1 module based boards
- Custom boards with ESP32-S3-WROOM-1/1U modules

**GPIO Availability:** Most development boards expose GPIO8, GPIO9, GPIO3, GPIO4 on headers.

### 6.2 RTC Module Compatibility

**Recommended RTC Modules:**
1. **Adafruit RV-3028 Breakout** (Product ID: 5188)
   - Includes pull-up resistors (check schematic)
   - Qwiic/STEMMA QT connectors
   - Battery holder for CR1220
   
2. **SparkFun Real Time Clock Module - RV-3028** (Product ID: BOB-16281)
   - Includes pull-up resistors
   - Qwiic connectors
   - Battery holder for CR1225

3. **Bare RV-3028-C7 Module**
   - Requires external pull-ups
   - More compact
   - Lower cost

**Important:** Check if breakout board includes pull-up resistors before adding external ones.

---

## 7. Thermal Considerations

### 7.1 Operating Temperature Range

**System Requirements:**
- Sensor nodes: -5°C to +8°C (one sensor in cold environment)
- Gateway: Typical indoor environment (15°C to 30°C)

**Component Ratings:**
- ESP32-S3: -40°C to +85°C ✓
- RV-3028-C7: -40°C to +85°C ✓
- Resistors/Capacitors: -55°C to +125°C (standard) ✓

**All components rated well above required range.**

### 7.2 RTC Temperature Compensation

**RV-3028-C7 Features:**
- Automatic temperature compensation
- Temperature sensor integrated
- Compensation range: -40°C to +85°C
- Accuracy maintained: ±1 ppm across full range

**No additional thermal management required.**

---

## 8. Conclusions and Recommendations

### 8.1 GPIO Pin Assignment (Final)

| Function | GPIO | Notes |
|----------|------|-------|
| I2C SDA | GPIO8 | Safe, no conflicts |
| I2C SCL | GPIO9 | Safe, no conflicts |
| RTC INT | GPIO3 | ext0 wake source |
| Button (Gateway) | GPIO4 | ext1 wake source |

### 8.2 Power Supply Recommendation

- Use shared 3.3V rail for development and testing
- Add CR2032 battery backup for production deployment
- Total deep sleep current: <20 µA (ESP32 + RTC + resistors)

### 8.3 Hardware Compatibility

- ESP32-S3 fully compatible with RV-3028-C7 RTC
- All GPIO pins available and suitable
- Operating temperature range: -40°C to +85°C (exceeds requirements)
- No hardware modifications required

### 8.4 Next Steps

1. Procure RV-3028-C7 RTC module (Adafruit or SparkFun recommended)
2. Verify pull-up resistors on breakout board
3. Prepare wiring connections per pin mapping
4. Proceed to Milestone 2: Firmware implementation

---

**Document Status:** Complete  
**Review Status:** Pending client review  
**Next Document:** RTC Component Recommendation (02_rtc_recommendation.md)

