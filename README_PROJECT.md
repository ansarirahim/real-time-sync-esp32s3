# ESP-NOW RTC Synchronization Project

**Developer:** Abdul Raheem Ansari
**Email:** ansarirahim1@gmail.com | **WhatsApp:** +91 9024304883
**LinkedIn:** [linkedin.com/in/abdul-raheem-ansari-a6871320](https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/)
**GitHub:** [github.com/ansarirahim/real-time-sync-esp32s3](https://github.com/ansarirahim/real-time-sync-esp32s3)

---

Real-time clock synchronization system for ESP32-S3 nodes using external RTC modules and ESP-NOW protocol.

## Project Overview

This project implements a low-power, high-accuracy time synchronization system for ESP32-based IoT sensor networks. It addresses the clock drift problem inherent in ESP32's internal RTC during deep sleep cycles by using external RTC modules (RV-3028-C7) with crystal oscillators.

### Key Features

- External RTC (RV-3028-C7) for accurate timekeeping during deep sleep
- ESP-NOW wireless synchronization protocol
- Ultra-low power consumption (<100 µA in deep sleep)
- Wake-up accuracy: ±50-100 ms (vs. several seconds with internal RTC)
- Dual wake sources for gateway (RTC alarm + manual button)
- Absolute time scheduling (prevents drift from manual wake events)
- Temperature range: -40°C to +85°C
- Battery life: Months to years depending on wake interval

### Hardware

- **Gateway:** ESP32-S3 + RV-3028-C7 RTC + Wake button
- **Sensor Nodes:** ESP32-S3/C3 + RV-3028-C7 RTC
- **Communication:** ESP-NOW (2.4 GHz)
- **Power:** 3.3V, <100 µA deep sleep current

## Repository Structure

```
.
├── main/                   # Main application code
├── components/             # Reusable components
│   ├── rtc_driver/        # RTC driver (RV-3028-C7)
│   ├── espnow_sync/       # ESP-NOW synchronization protocol
│   └── sleep_manager/     # Deep sleep management
├── docs/                   # Documentation
│   ├── milestone1/        # M1: Hardware analysis & RTC recommendation
│   ├── milestone2/        # M2: Firmware implementation
│   └── milestone3/        # M3: Testing & validation
├── hardware/               # Hardware schematics and BOM
├── test_data/             # Test logs and measurements
└── README.md              # This file
```

## Milestones

### Milestone 1: Review & RTC Recommendation (2 days, $40)
- Hardware analysis (ESP32-S3 GPIO availability)
- RTC component recommendation (RV-3028-C7 vs AB1805 vs DS3231)
- Electrical connection plan (pin mapping, pull-ups, decoupling)
- Power analysis and battery life projection
- Bill of Materials (BOM)

**Branch:** `milestone/m1-hardware-analysis`

### Milestone 2: Firmware Implementation (4-5 days, $70)
- RTC driver library (I2C communication, alarm configuration)
- ESP-NOW synchronization protocol
- Deep sleep logic (gateway and sensors)
- Dual wake source handling (RTC alarm + button)
- Absolute time scheduling
- Test logs and documentation

**Branch:** `milestone/m2-firmware-implementation`

### Milestone 3: Testing & Validation (3 days, $40)
- 12-hour timing validation test
- Power consumption measurements
- Wake window optimization
- Temperature range testing (-5°C to +8°C)
- Final firmware binaries
- Complete project report

**Branch:** `milestone/m3-testing-validation`

## Build Instructions

### Prerequisites

- ESP-IDF v5.0 or later
- ESP32-S3 development board
- RV-3028-C7 RTC module
- USB cable for programming

### Setup ESP-IDF

```bash
# Install ESP-IDF (if not already installed)
# Follow: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/

# Set up ESP-IDF environment
. $HOME/esp/esp-idf/export.sh  # Linux/Mac
# OR
%userprofile%\esp\esp-idf\export.bat  # Windows
```

### Build and Flash

```bash
# Configure project for ESP32-S3
idf.py set-target esp32s3

# Build project
idf.py build

# Flash to device
idf.py -p COM3 flash monitor  # Windows
# OR
idf.py -p /dev/ttyUSB0 flash monitor  # Linux
```

## Hardware Connection

### ESP32-S3 to RV-3028-C7 RTC

| ESP32-S3 Pin | RTC Pin | Function | Notes |
|--------------|---------|----------|-------|
| 3V3 | VCC | Power | 3.3V supply |
| GND | GND | Ground | Common ground |
| GPIO8 | SDA | I2C Data | 4.7kΩ pull-up |
| GPIO9 | SCL | I2C Clock | 4.7kΩ pull-up |
| GPIO3 | INT | Interrupt | 10kΩ pull-up, wake source |

### Gateway Additional Connection

| ESP32-S3 Pin | Component | Function | Notes |
|--------------|-----------|----------|-------|
| GPIO4 | Button | Manual Wake | 10kΩ pull-down to GND |

## Testing

### Unit Tests

```bash
# Run component tests
idf.py build
idf.py -p COM3 flash monitor
```

### Integration Tests

See `docs/milestone3/test_plan.md` for detailed test procedures.

## Project Status

- [x] Milestone 1: Hardware Analysis & RTC Recommendation
- [ ] Milestone 2: Firmware Implementation
- [ ] Milestone 3: Testing & Validation

## License

Proprietary - Client Project for Orbikey Bilisim (Ridvan)

## Contact & Support

**Abdul Raheem Ansari**
Embedded Systems Engineer | IoT Solutions Developer

- **Email:** ansarirahim1@gmail.com
- **WhatsApp:** +91 9024304883
- **LinkedIn:** [linkedin.com/in/abdul-raheem-ansari-a6871320](https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/)
- **GitHub:** [github.com/ansarirahim](https://github.com/ansarirahim)

## References

- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [RV-3028-C7 Datasheet](https://www.microcrystal.com/en/products/real-time-clock-rtc-modules/rv-3028-c7/)
- [ESP-NOW Protocol](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/network/esp_now.html)

