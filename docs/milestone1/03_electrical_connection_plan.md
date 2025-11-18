# Milestone 1: Electrical Connection Plan

## Detailed Wiring and Component Specifications

**Project:** ESP-NOW RTC Synchronization System  
**Date:** 2024-11-18  
**Author:** Abdul Raheem Ansari  
**Version:** 1.0.0-M1

---

## 1. Pin Mapping

### 1.1 Sensor Node Configuration

**ESP32-S3 to RV-3028-C7 RTC**

| ESP32-S3 Pin | RTC Pin | Signal Name | Direction | Function |
|--------------|---------|-------------|-----------|----------|
| 3V3 | VCC | Power | Output | 3.3V power supply |
| GND | GND | Ground | - | Common ground |
| GPIO8 | SDA | I2C Data | Bidirectional | I2C data line |
| GPIO9 | SCL | I2C Clock | Output | I2C clock line |
| GPIO3 | INT | Interrupt | Input | RTC alarm wake-up |

**Total Connections:** 5 wires

### 1.2 Gateway Node Configuration

**ESP32-S3 to RV-3028-C7 RTC + Button**

| ESP32-S3 Pin | Component | Signal Name | Direction | Function |
|--------------|-----------|-------------|-----------|----------|
| 3V3 | RTC VCC | Power | Output | 3.3V power supply |
| GND | RTC GND | Ground | - | Common ground |
| GPIO8 | RTC SDA | I2C Data | Bidirectional | I2C data line |
| GPIO9 | RTC SCL | I2C Clock | Output | I2C clock line |
| GPIO3 | RTC INT | Interrupt | Input | RTC alarm wake-up |
| GPIO4 | Button | Button Input | Input | Manual wake button |
| GND | Button GND | Ground | - | Button ground |

**Total Connections:** 7 wires (5 for RTC + 2 for button)

---

## 2. Pull-up/Pull-down Resistor Configuration

### 2.1 I2C Bus Pull-up Resistors

**SDA and SCL Lines:**
- **Resistor Value:** 4.7 kΩ
- **Quantity:** 2 (one for SDA, one for SCL)
- **Connection:** Between signal line and 3.3V
- **Power Rating:** 1/8W or 1/4W
- **Package:** 0805 SMD or through-hole

**Calculation:**
```
V_DD = 3.3V
I_pull-up = V_DD / R = 3.3V / 4.7kΩ = 0.7 mA (when line is LOW)
P_dissipation = I² × R = (0.7mA)² × 4.7kΩ = 2.3 mW (negligible)
```

**Note:** Check if RTC breakout board includes pull-up resistors. If present, external resistors may not be needed.

**Adafruit RV-3028 Breakout:** Includes 10kΩ pull-ups (check schematic). May need to add 4.7kΩ in parallel for faster I2C or remove board resistors.

### 2.2 RTC INT Pin Pull-up Resistor

**INT Line:**
- **Resistor Value:** 10 kΩ
- **Quantity:** 1
- **Connection:** Between INT pin and 3.3V
- **Power Rating:** 1/8W or 1/4W
- **Package:** 0805 SMD or through-hole

**Rationale:**
- RTC INT pin is open-drain output
- Requires external pull-up to 3.3V
- 10kΩ provides good noise immunity while minimizing power consumption

**Power Consumption:**
```
I_pull-up = V_DD / R = 3.3V / 10kΩ = 0.33 mA (when INT is LOW)
INT is LOW only during alarm (brief period), so average current is negligible
```

### 2.3 Button Pull-down Resistor (Gateway Only)

**Button Input (GPIO4):**
- **Resistor Value:** 10 kΩ
- **Quantity:** 1
- **Connection:** Between GPIO4 and GND
- **Power Rating:** 1/8W or 1/4W
- **Package:** 0805 SMD or through-hole

**Rationale:**
- Keeps GPIO4 LOW when button is not pressed
- Prevents floating input
- When button is pressed, GPIO4 is pulled HIGH to 3.3V

**Power Consumption:**
```
I_pull-down = V_DD / R = 3.3V / 10kΩ = 0.33 mA (when button is pressed)
Button press is brief and infrequent, so average current is negligible
```

---

## 3. Decoupling Capacitors

### 3.1 RTC Power Supply Decoupling

**100 nF Ceramic Capacitor:**
- **Value:** 100 nF (0.1 µF)
- **Voltage Rating:** 6.3V or higher (10V recommended)
- **Type:** Ceramic (X7R or X5R dielectric)
- **Package:** 0805 SMD or through-hole
- **Placement:** As close as possible to RTC VCC pin (<5 mm)

**Purpose:**
- Filter high-frequency noise on power supply
- Provide local charge reservoir for I2C communication bursts
- Improve RTC stability

### 3.2 ESP32-S3 Power Supply Decoupling

**10 µF Electrolytic + 100 nF Ceramic:**
- **10 µF:** Bulk capacitance for WiFi bursts
- **100 nF:** High-frequency filtering
- **Placement:** Near ESP32-S3 VDD pins

**Note:** Most ESP32-S3 development boards include adequate decoupling. Verify on schematic.

---

## 4. Wiring Diagrams

### 4.1 Sensor Node Wiring

```
ESP32-S3                    RV-3028-C7 RTC
┌─────────┐                 ┌──────────┐
│         │                 │          │
│  3V3    ├─────────────────┤ VCC      │
│         │                 │          │
│  GND    ├─────────────────┤ GND      │
│         │                 │          │
│  GPIO8  ├────┬────────────┤ SDA      │
│  (SDA)  │    │            │          │
│         │   [4.7kΩ]       │          │
│         │    │            │          │
│  GPIO9  ├────┼───┬────────┤ SCL      │
│  (SCL)  │    │   │        │          │
│         │   [4.7kΩ]       │          │
│         │    │   │        │          │
│  GPIO3  ├────┼───┼───┬────┤ INT      │
│  (INT)  │    │   │   │    │          │
│         │    │   │  [10kΩ]│          │
│         │    │   │   │    │          │
│  3V3    ├────┴───┴───┴────┘          │
│         │                 │          │
└─────────┘                 └──────────┘

[4.7kΩ] = Pull-up resistor to 3.3V
[10kΩ]  = Pull-up resistor to 3.3V
```

### 4.2 Gateway Node Wiring

```
ESP32-S3                    RV-3028-C7 RTC
┌─────────┐                 ┌──────────┐
│         │                 │          │
│  3V3    ├─────────────────┤ VCC      │
│         │                 │          │
│  GND    ├─────────────────┤ GND      │
│         │                 │          │
│  GPIO8  ├────┬────────────┤ SDA      │
│  (SDA)  │    │            │          │
│         │   [4.7kΩ]       │          │
│         │    │            │          │
│  GPIO9  ├────┼───┬────────┤ SCL      │
│  (SCL)  │    │   │        │          │
│         │   [4.7kΩ]       │          │
│         │    │   │        │          │
│  GPIO3  ├────┼───┼───┬────┤ INT      │
│  (INT)  │    │   │   │    │          │
│         │    │   │  [10kΩ]│          │
│         │    │   │   │    │          │
│  3V3    ├────┴───┴───┴────┘          │
│         │                            │
│  GPIO4  ├────┬─────[Button]──── 3V3 │
│ (Button)│    │                       │
│         │   [10kΩ]                   │
│         │    │                       │
│  GND    ├────┴───────────────────────┘
│         │
└─────────┘

[4.7kΩ] = Pull-up resistor to 3.3V
[10kΩ]  = Pull-up/pull-down resistor
[Button] = Normally-open push button
```

---

## 5. Component Specifications

### 5.1 Resistors

| Designator | Value | Tolerance | Power | Package | Quantity (Sensor) | Quantity (Gateway) |
|------------|-------|-----------|-------|---------|-------------------|--------------------|
| R1 | 4.7 kΩ | ±5% | 1/8W | 0805 SMD | 1 | 1 |
| R2 | 4.7 kΩ | ±5% | 1/8W | 0805 SMD | 1 | 1 |
| R3 | 10 kΩ | ±5% | 1/8W | 0805 SMD | 1 | 1 |
| R4 | 10 kΩ | ±5% | 1/8W | 0805 SMD | 0 | 1 |

**Total Resistors:**
- Sensor Node: 3
- Gateway Node: 4

### 5.2 Capacitors

| Designator | Value | Voltage | Type | Package | Quantity (Sensor) | Quantity (Gateway) |
|------------|-------|---------|------|---------|-------------------|--------------------|
| C1 | 100 nF | 10V | Ceramic X7R | 0805 SMD | 1 | 1 |
| C2 | 10 µF | 10V | Electrolytic | Radial | 1 | 1 |

**Total Capacitors:**
- Sensor Node: 2
- Gateway Node: 2

### 5.3 Button (Gateway Only)

| Component | Type | Specifications | Quantity |
|-----------|------|----------------|----------|
| SW1 | Tactile Push Button | SPST, Normally Open, 50mA, 12V | 1 |

**Recommended:** 6mm x 6mm tactile switch (standard size)

---

## 6. PCB Layout Guidelines

### 6.1 I2C Trace Routing

**Best Practices:**
- Keep SDA and SCL traces short (<10 cm for 100 kHz, <5 cm for 400 kHz)
- Route SDA and SCL traces parallel and close together
- Avoid routing I2C traces near high-frequency signals (WiFi antenna, SPI flash)
- Use ground plane for return path
- Minimize trace capacitance (<200 pF total bus capacitance)

**Trace Width:**
- Minimum: 0.2 mm (8 mil)
- Recommended: 0.3 mm (12 mil) for easier manufacturing

### 6.2 Component Placement

**Critical Placements:**
1. **100 nF decoupling capacitor:** <5 mm from RTC VCC pin
2. **Pull-up resistors:** Close to I2C lines (within 10 mm)
3. **RTC module:** Away from WiFi antenna (>20 mm separation)
4. **Button:** Accessible on enclosure edge

### 6.3 Ground Plane

- Use solid ground plane on bottom layer
- Connect all GND pins to ground plane with vias
- Minimize ground loop area

---

## 7. Assembly Instructions

### 7.1 Breadboard Prototype (Development)

**Step 1:** Connect Power
1. Connect ESP32-S3 3V3 pin to breadboard power rail
2. Connect ESP32-S3 GND pin to breadboard ground rail
3. Connect RTC VCC to power rail
4. Connect RTC GND to ground rail

**Step 2:** Install Pull-up Resistors
1. Install 4.7kΩ resistor between SDA and 3V3
2. Install 4.7kΩ resistor between SCL and 3V3
3. Install 10kΩ resistor between INT and 3V3

**Step 3:** Connect I2C Lines
1. Connect ESP32-S3 GPIO8 to RTC SDA
2. Connect ESP32-S3 GPIO9 to RTC SCL

**Step 4:** Connect Interrupt Line
1. Connect ESP32-S3 GPIO3 to RTC INT

**Step 5:** (Gateway Only) Connect Button
1. Install 10kΩ resistor between GPIO4 and GND
2. Connect button between GPIO4 and 3V3

**Step 6:** Add Decoupling Capacitor
1. Place 100 nF capacitor between RTC VCC and GND (close to RTC)

### 7.2 Using Adafruit RV-3028 Breakout Board

**Simplified Wiring (Breakout includes pull-ups):**

| ESP32-S3 Pin | Breakout Pin | Wire Color (Suggested) |
|--------------|--------------|------------------------|
| 3V3 | VIN | Red |
| GND | GND | Black |
| GPIO8 | SDA | Blue |
| GPIO9 | SCL | Yellow |
| GPIO3 | INT | Green |

**Note:** Adafruit breakout includes 10kΩ pull-up resistors on SDA, SCL, and INT. External pull-ups not required.

**For Gateway:** Add button between GPIO4 and 3V3 with 10kΩ pull-down to GND.

---

## 8. Testing and Verification

### 8.1 Visual Inspection

- [ ] All connections match pin mapping table
- [ ] Pull-up resistors installed correctly
- [ ] Decoupling capacitor placed close to RTC
- [ ] No short circuits between adjacent pins
- [ ] Button (gateway) wired correctly

### 8.2 Continuity Testing

- [ ] 3V3 to RTC VCC: Continuity
- [ ] GND to RTC GND: Continuity
- [ ] GPIO8 to RTC SDA: Continuity
- [ ] GPIO9 to RTC SCL: Continuity
- [ ] GPIO3 to RTC INT: Continuity
- [ ] 3V3 to SDA (through 4.7kΩ): ~4.7kΩ resistance
- [ ] 3V3 to SCL (through 4.7kΩ): ~4.7kΩ resistance
- [ ] 3V3 to INT (through 10kΩ): ~10kΩ resistance

### 8.3 Power Supply Testing

- [ ] Measure voltage at RTC VCC: Should be 3.3V ±0.1V
- [ ] Measure current draw (ESP32 + RTC): <50 mA idle, <200 mA during WiFi

---

## 9. Troubleshooting

### 9.1 I2C Communication Failures

**Symptom:** RTC not detected on I2C bus

**Possible Causes:**
1. Incorrect wiring (SDA/SCL swapped)
2. Missing or incorrect pull-up resistors
3. I2C address mismatch (should be 0x52 for RV-3028-C7)
4. Power supply issue (RTC not powered)

**Solutions:**
1. Verify wiring with multimeter (continuity test)
2. Check pull-up resistors (should measure ~4.7kΩ to 3V3)
3. Scan I2C bus with i2cdetect tool
4. Measure RTC VCC (should be 3.3V)

### 9.2 Wake-up Not Working

**Symptom:** ESP32 does not wake from deep sleep

**Possible Causes:**
1. INT pin not configured as wake source in firmware
2. Missing pull-up resistor on INT pin
3. RTC alarm not configured correctly

**Solutions:**
1. Verify `esp_sleep_enable_ext0_wakeup(GPIO_NUM_3, 0)` in firmware
2. Check 10kΩ pull-up resistor on INT pin
3. Verify RTC alarm registers are set correctly

---

**Document Status:** Complete  
**Review Status:** Pending client review  
**Next Document:** Power Analysis and Battery Life Projection (04_power_analysis.md)

