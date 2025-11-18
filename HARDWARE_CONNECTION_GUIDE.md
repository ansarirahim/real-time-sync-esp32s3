# Hardware Connection Guide — RTC Integration with ESP32

**Project:** ESP32-S3 Real-Time Sync Engineering
**Document:** RTC Hardware Integration Guide
**Date:** November 18, 2024
**Version:** 1.0.0

**Author:** A.R. Ansari
**Email:** ansarirahim1@gmail.com
**WhatsApp:** +919024304833
**LinkedIn:** https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/

---

## RTC Module Recommendation: RV-3028-C7

### Why RV-3028-C7?
- Ultra-low power: **45 nA** typical in timekeeping mode
- High accuracy: **±1 ppm** with automatic temperature compensation
- Wide temperature range: **-40°C to +85°C**
- I²C interface (400 kHz Fast Mode)
- Programmable alarm with interrupt output
- Integrated 32.768 kHz crystal (no external crystal needed!)
- Small footprint: 3.2mm × 1.5mm (SMD) or breakout board available

### Alternative: AB1805
- Similar specifications, slightly higher power (22 nA)
- Requires external 32.768 kHz crystal
- Good alternative if RV-3028 is unavailable

---

## Pin Connections

### For ESP32-C3 (Sensor Nodes)

| RTC Pin | ESP32-C3 Pin | Function | Notes |
|---------|--------------|----------|-------|
| VCC | 3V3 | Power Supply | 3.3V rail |
| GND | GND | Ground | Common ground |
| SDA | GPIO8 | I²C Data | Default I²C SDA |
| SCL | GPIO9 | I²C Clock | Default I²C SCL |
| INT | GPIO2 | Interrupt | RTC alarm wake signal |

**Pull-up Resistors:**
- SDA: 4.7kΩ to 3.3V (often built into breakout boards)
- SCL: 4.7kΩ to 3.3V (often built into breakout boards)
- INT: 10kΩ to 3.3V (for stable interrupt detection)

---

### For ESP32-S3 (Gateway Node)

| RTC Pin | ESP32-S3 Pin | Function | Notes |
|---------|--------------|----------|-------|
| VCC | 3V3 | Power Supply | 3.3V rail |
| GND | GND | Ground | Common ground |
| SDA | GPIO8 | I²C Data | Default I²C SDA |
| SCL | GPIO9 | I²C Clock | Default I²C SCL |
| INT | GPIO3 | Interrupt | RTC alarm wake signal (gateway also sleeps!) |
| — | GPIO4 | Button Input | Manual wake button (see below) |

**Pull-up Resistors:**
- SDA: 4.7kΩ to 3.3V
- SCL: 4.7kΩ to 3.3V
- INT: 10kΩ to 3.3V

---

## Schematic Diagram (Text Representation)

```
ESP32-C3/S3                    RV-3028-C7 RTC Module
┌─────────────┐                ┌──────────────┐
│             │                │              │
│         3V3 ├────────────────┤ VCC          │
│             │                │              │
│         GND ├────────────────┤ GND          │
│             │                │              │
│             │     ┌──4.7kΩ───┤ SDA          │
│   GPIO8(SDA)├─────┤          │              │
│             │     │          │              │
│             │     ┌──4.7kΩ───┤ SCL          │
│   GPIO9(SCL)├─────┤          │              │
│             │     │          │              │
│             │     ┌──10kΩ────┤ INT (Alarm)  │
│   GPIO2/3   ├─────┤          │              │
│    (INT)    │     │          │              │
│             │     │          │              │
└─────────────┘     │          └──────────────┘
                    │
                   3V3
```

---

## Power Supply Considerations

### Decoupling Capacitors
- **100nF ceramic capacitor** between VCC and GND (close to RTC module)
- **10µF electrolytic capacitor** on main 3.3V rail (for stability)

### Backup Battery (Optional)
- RV-3028-C7 supports backup battery (CR1225 or similar)
- Allows RTC to maintain time even when main power is off
- **Not required** for this application (nodes powered continuously)

---

## Manual Wake Button (Gateway Only)

### Button Connection

The gateway includes a physical button for manual wake-up to display GUI:

```
ESP32-S3 GPIO4
      │
      ├──── Button ──── 3.3V
      │
      └──── 10kΩ ──── GND (pull-down)
```

**When button is pressed:** GPIO4 goes HIGH, triggering wake-up from deep sleep.

### Button Specifications
- **Type:** Momentary push button (normally open)
- **Pull-down resistor:** 10kΩ to GND
- **Wake trigger:** Rising edge (LOW to HIGH)
- **Debouncing:** Handled in firmware (10-50 ms delay)

### Important: Button Wake Does NOT Affect Sync

The firmware uses **absolute time scheduling** to ensure manual wake events do not shift the scheduled transmission times. See `BUTTON_WAKE_SOLUTION.md` for detailed implementation.

---

## Deep Sleep Wake-Up Configuration

### ESP32 Wake-Up Source (Sensor Nodes)
The RTC interrupt pin (INT) will be configured as an **external wake-up source** for ESP32 deep sleep.

**ESP-IDF Code Example:**
```c
// Configure RTC INT pin as wake-up source
esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, 0);  // Wake on LOW (falling edge)

// Enter deep sleep
esp_deep_sleep_start();
```

**Wake-Up Behavior:**
1. RTC alarm triggers at programmed time
2. RTC pulls INT pin LOW
3. ESP32 wakes from deep sleep
4. Firmware reads boot reason and proceeds with ESP-NOW communication
5. After data exchange, ESP32 sets next RTC alarm and returns to sleep

---

### ESP32 Wake-Up Sources (Gateway Node)

The gateway has **two wake sources**:

**ESP-IDF Code Example:**
```c
// Wake source 1: RTC alarm (scheduled transmission)
esp_sleep_enable_ext0_wakeup(GPIO_NUM_3, 0);  // RTC INT pin, wake on LOW

// Wake source 2: Button press (manual wake for GUI)
uint64_t button_mask = (1ULL << GPIO_NUM_4);
esp_sleep_enable_ext1_wakeup(button_mask, ESP_EXT1_WAKEUP_ANY_HIGH);

// Enter deep sleep
esp_deep_sleep_start();
```

**Wake-Up Behavior:**
1. **RTC Alarm Wake:** Handle scheduled transmission, receive sensor data, forward to server
2. **Button Wake:** Show GUI, display current data, return to sleep without affecting schedule
3. Firmware detects wake reason using `esp_sleep_get_wakeup_cause()`

---

## I²C Configuration

### I²C Address
- **RV-3028-C7 default address:** `0x52` (7-bit address)

### I²C Speed
- **Standard Mode:** 100 kHz (safe, lower power)
- **Fast Mode:** 400 kHz (faster initialization, slightly higher power)
- **Recommendation:** Use 100 kHz for battery-powered nodes

### ESP-IDF I²C Initialization Example
```c
i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = GPIO_NUM_8,
    .scl_io_num = GPIO_NUM_9,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 100000,  // 100 kHz
};
i2c_param_config(I2C_NUM_0, &conf);
i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
```

---

## Bill of Materials (BOM)

| Component | Part Number | Quantity per Node | Supplier | Est. Cost |
|-----------|-------------|-------------------|----------|-----------|
| RTC Module | RV-3028-C7 Breakout | 1 | Adafruit, SparkFun, Mouser | $5-8 |
| Resistor 4.7kΩ | 0805 SMD or through-hole | 2 | Any | $0.10 |
| Resistor 10kΩ | 0805 SMD or through-hole | 1 | Any | $0.05 |
| Capacitor 100nF | 0805 SMD or through-hole | 1 | Any | $0.05 |
| Capacitor 10µF | Electrolytic or ceramic | 1 | Any | $0.10 |
| **Total per node** | | | | **~$5.30** |

**Note:** If using RV-3028-C7 breakout boards (e.g., from Adafruit or SparkFun), pull-up resistors are often already included.

---

## Recommended RTC Breakout Boards

### Option 1: Adafruit RV-3028 Breakout
- **Product:** Adafruit RV-3028 Real Time Clock (RTC) Breakout
- **Features:** Built-in pull-ups, STEMMA QT / Qwiic connectors
- **Link:** https://www.adafruit.com/product/5188
- **Cost:** ~$7.95

### Option 2: SparkFun RV-3028 Breakout
- **Product:** SparkFun Real Time Clock Module - RV-3028 (Qwiic)
- **Features:** Qwiic connectors, easy plug-and-play
- **Link:** https://www.sparkfun.com/products/16281
- **Cost:** ~$6.95

### Option 3: Bare RV-3028-C7 IC (for custom PCB)
- **Supplier:** Mouser, Digikey
- **Part Number:** RV-3028-C7-32.768kHz-3ppm-TA-QC
- **Cost:** ~$2-3 per unit (bulk pricing available)

---

## Assembly Instructions

### Step 1: Solder or Connect RTC Module
- If using breakout board: Connect via jumper wires or solder headers
- If using bare IC: Solder to custom PCB with proper decoupling

### Step 2: Verify I²C Connection
- Use I²C scanner sketch to detect RTC at address `0x52`
- Ensure SDA/SCL lines are not swapped

### Step 3: Test RTC Functionality
- Set time and read it back
- Configure alarm and verify INT pin goes LOW when alarm triggers

### Step 4: Test Deep Sleep Wake-Up
- Configure ESP32 to wake on INT pin
- Enter deep sleep and verify wake-up on RTC alarm

---

## Important: Gateway Sleep Behavior

**The gateway node also enters deep sleep between communication windows.**

This is a critical design decision for battery-powered operation:

- **Gateway is NOT always awake** — it sleeps just like the sensor nodes
- **Gateway wakes up at scheduled intervals** triggered by its RTC alarm
- **During wake window:** Gateway initializes ESP-NOW, listens for sensor data, sends acknowledgments, then returns to sleep
- **Power savings:** Gateway deep sleep current is also <100 µA (same as sensors)
- **Battery life:** Gateway can run for months/years on battery power

**Why this matters:**
- If gateway stayed awake continuously, it would consume 80-150 mA (WiFi/ESP-NOW active)
- With sleep cycles, average current drops to <1 mA (depending on wake interval)
- This extends battery life from days to months/years

**Synchronization:**
- All nodes (gateway + sensors) wake up at the same scheduled time (±50-100 ms skew)
- Gateway wakes slightly earlier or stays awake slightly longer to ensure it's ready when sensors transmit
- After communication window closes, gateway returns to deep sleep

---

## Troubleshooting

### Issue: I²C device not detected
- **Check:** Wiring (SDA/SCL not swapped)
- **Check:** Pull-up resistors present (4.7kΩ to 3.3V)
- **Check:** RTC module powered (3.3V on VCC pin)

### Issue: ESP32 does not wake from deep sleep
- **Check:** INT pin configured as wake source (`esp_sleep_enable_ext0_wakeup`)
- **Check:** RTC alarm properly configured
- **Check:** INT pin pull-up resistor present (10kΩ)

### Issue: RTC loses time after power cycle
- **Check:** Backup battery installed (if required)
- **Note:** For this project, time will be re-synced via ESP-NOW on power-up

---

## Next Steps
Once hardware is connected, proceed to **Milestone 2** for firmware integration and synchronization protocol implementation.

---

## Document Information

**Author:** A.R. Ansari
**Email:** ansarirahim1@gmail.com
**WhatsApp:** +919024304833
**LinkedIn:** https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/
**Date:** November 18, 2024

**Copyright:** © 2024 A.R. Ansari. All rights reserved.

