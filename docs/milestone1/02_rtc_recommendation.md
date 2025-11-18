# Milestone 1: RTC Component Recommendation

## External RTC Module Selection and Comparison

**Project:** ESP-NOW RTC Synchronization System  
**Date:** 2024-11-18  
**Author:** Abdul Raheem Ansari  
**Version:** 1.0.0-M1

---

## 1. Executive Summary

**Recommended RTC:** **RV-3028-C7**

**Justification:**
- Lowest power consumption (45 nA timekeeping)
- Highest accuracy (±1 ppm with temperature compensation)
- Integrated crystal (no external components needed)
- Wide temperature range (-40°C to +85°C)
- I2C interface (compatible with ESP32-S3)
- Alarm functionality with interrupt output
- Best value for battery-powered applications

---

## 2. RTC Comparison Matrix

### 2.1 Candidate RTCs

| Feature | RV-3028-C7 | AB1805 | DS3231 |
|---------|------------|--------|--------|
| **Manufacturer** | Micro Crystal | Abracon | Maxim Integrated |
| **Interface** | I2C | I2C/SPI | I2C |
| **I2C Address** | 0x52 | 0x69 | 0x68 |
| **Supply Voltage** | 1.1V - 5.5V | 1.2V - 5.5V | 2.3V - 5.5V |
| **Timekeeping Current** | **45 nA** | 22 nA | 840 nA |
| **Accuracy (no comp)** | ±5 ppm | ±20 ppm | ±2 ppm |
| **Accuracy (with comp)** | **±1 ppm** | ±3 ppm | ±2 ppm |
| **Temp Range** | -40°C to +85°C | -40°C to +85°C | -40°C to +85°C |
| **Crystal** | **Integrated** | External required | **Integrated** |
| **Alarm Outputs** | 1 | 2 | 2 |
| **Battery Backup** | Yes | Yes | Yes |
| **Package** | 3.2x1.5mm | 4.9x3.0mm | 16-pin SOIC |
| **Breakout Availability** | Excellent | Good | Excellent |
| **Cost (breakout)** | $6-8 | $8-10 | $4-6 |
| **Cost (IC only)** | $2-3 | $3-4 | $3-5 |

### 2.2 Detailed Analysis

#### RV-3028-C7 (RECOMMENDED)

**Pros:**
- Ultra-low power: 45 nA (battery life: 10+ years on CR2032)
- Integrated crystal: No external 32.768 kHz crystal needed
- Automatic temperature compensation: ±1 ppm accuracy
- Wide voltage range: 1.1V - 5.5V (flexible power supply)
- Small footprint: 3.2 x 1.5 mm
- Excellent breakout boards available (Adafruit, SparkFun)
- I2C fast mode: up to 400 kHz
- Alarm with interrupt output
- Trickle charger for supercapacitor backup

**Cons:**
- Slightly higher power than AB1805 (but negligible difference)
- Single alarm output (vs. 2 for AB1805/DS3231)
- Higher cost than DS3231

**Best For:** Battery-powered applications requiring ultra-low power and high accuracy

---

#### AB1805

**Pros:**
- Lowest power: 22 nA timekeeping current
- Dual alarm outputs
- I2C and SPI interfaces
- Automatic temperature compensation: ±3 ppm
- Integrated power management

**Cons:**
- **Requires external 32.768 kHz crystal** (adds cost, complexity, board space)
- Crystal selection critical (ESR, load capacitance, temperature stability)
- More complex PCB layout (crystal traces must be short, low capacitance)
- Higher cost overall (IC + crystal + load capacitors)
- Less common breakout boards

**Best For:** Applications requiring dual alarms or SPI interface

---

#### DS3231

**Pros:**
- Integrated crystal (no external components)
- Good accuracy: ±2 ppm
- Widely available and well-documented
- Lower cost breakout boards
- Dual alarm outputs
- Aging trim register

**Cons:**
- **High power consumption:** 840 nA (18x higher than RV-3028-C7)
- Battery life: ~1-2 years on CR2032 (vs. 10+ years for RV-3028-C7)
- Larger package: 16-pin SOIC
- Higher minimum voltage: 2.3V (less flexible)
- No trickle charger

**Best For:** Mains-powered applications or where battery life is not critical

---

## 3. Power Consumption Analysis

### 3.1 Battery Life Calculation

**Assumptions:**
- Battery: CR2032 (220 mAh capacity)
- RTC only (ESP32 in deep sleep, wakes periodically)
- 80% battery efficiency

| RTC Model | Current | Battery Life | Notes |
|-----------|---------|--------------|-------|
| RV-3028-C7 | 45 nA | **~44 years** | Practical limit: battery shelf life (~10 years) |
| AB1805 | 22 nA | **~90 years** | Practical limit: battery shelf life (~10 years) |
| DS3231 | 840 nA | **~2.4 years** | Actual battery life |

**Conclusion:** RV-3028-C7 and AB1805 both exceed battery shelf life. DS3231 requires battery replacement every 2-3 years.

### 3.2 System Power Budget

**Deep Sleep Mode (ESP32 + RTC):**

| Component | RV-3028-C7 | AB1805 | DS3231 |
|-----------|------------|--------|--------|
| ESP32-S3 deep sleep | 5-10 µA | 5-10 µA | 5-10 µA |
| RTC timekeeping | 0.045 µA | 0.022 µA | 0.84 µA |
| Pull-up resistors (3x) | ~2 µA | ~2 µA | ~2 µA |
| **Total** | **~7-12 µA** | **~7-12 µA** | **~8-13 µA** |

**Key Insight:** ESP32 deep sleep current dominates. RTC power consumption is negligible (<1% of total).

---

## 4. Accuracy and Drift Analysis

### 4.1 Clock Drift Over Time

**Scenario:** 1-hour sleep cycle (sensor wakes every hour)

| RTC Model | Accuracy | Drift per Hour | Drift per Day | Drift per Week |
|-----------|----------|----------------|---------------|----------------|
| RV-3028-C7 | ±1 ppm | ±3.6 ms | ±86 ms | ±604 ms |
| AB1805 | ±3 ppm | ±10.8 ms | ±259 ms | ±1.8 s |
| DS3231 | ±2 ppm | ±7.2 ms | ±173 ms | ±1.2 s |
| ESP32 internal | ±50-100 ppm | ±180-360 ms | ±4.3-8.6 s | ±30-60 s |

**Wake Window Requirement:** ±50-100 ms for reliable ESP-NOW communication

**Analysis:**
- **RV-3028-C7:** Drift per hour (±3.6 ms) is well within wake window tolerance
- **AB1805:** Drift per hour (±10.8 ms) is acceptable
- **DS3231:** Drift per hour (±7.2 ms) is acceptable
- **ESP32 internal:** Drift per hour (±180-360 ms) exceeds wake window tolerance

**Conclusion:** All external RTCs meet accuracy requirements. RV-3028-C7 provides best margin.

### 4.2 Temperature Stability

**Operating Range:** -5°C to +8°C (coldest sensor node)

| RTC Model | Temp Compensation | Accuracy at -5°C | Accuracy at +8°C |
|-----------|-------------------|------------------|------------------|
| RV-3028-C7 | Automatic | ±1 ppm | ±1 ppm |
| AB1805 | Automatic | ±3 ppm | ±3 ppm |
| DS3231 | Automatic (TCXO) | ±2 ppm | ±2 ppm |
| ESP32 internal | None | ±100-200 ppm | ±50-100 ppm |

**Conclusion:** All external RTCs maintain accuracy across required temperature range.

---

## 5. Integration Complexity

### 5.1 Hardware Integration

| Aspect | RV-3028-C7 | AB1805 | DS3231 |
|--------|------------|--------|--------|
| Crystal | Integrated ✓ | External required ✗ | Integrated ✓ |
| PCB Complexity | Low | High | Low |
| External Components | Minimal | Crystal + caps | Minimal |
| Breakout Boards | Excellent | Limited | Excellent |
| Wiring | Simple | Simple | Simple |

**Winner:** RV-3028-C7 and DS3231 (integrated crystal)

### 5.2 Software Integration

| Aspect | RV-3028-C7 | AB1805 | DS3231 |
|--------|------------|--------|--------|
| I2C Interface | Standard | Standard | Standard |
| Register Map | Well-documented | Well-documented | Well-documented |
| Arduino Libraries | Available | Available | Extensive |
| ESP-IDF Examples | Available | Limited | Available |
| Alarm Configuration | Simple | Moderate | Simple |

**Winner:** DS3231 (most examples), but all are well-supported

---

## 6. Cost Analysis

### 6.1 Component Cost (per node)

| Item | RV-3028-C7 | AB1805 | DS3231 |
|------|------------|--------|--------|
| RTC IC | $2-3 | $3-4 | $3-5 |
| Crystal | Included | $0.50-1 | Included |
| Load Capacitors | - | $0.10 | - |
| Breakout Board | $6-8 | $8-10 | $4-6 |
| Pull-up Resistors | $0.10 | $0.10 | $0.10 |
| Decoupling Cap | $0.05 | $0.05 | $0.05 |
| **Total (breakout)** | **$6-8** | **$8-10** | **$4-6** |
| **Total (custom PCB)** | **$2-3** | **$4-5** | **$3-5** |

**Winner (breakout):** DS3231 (lowest cost)  
**Winner (custom PCB):** RV-3028-C7 (lowest cost, no crystal needed)

### 6.2 Total Cost of Ownership

**Factors:**
- Initial component cost
- Battery replacement cost
- Labor for battery replacement
- System downtime during battery replacement

| RTC Model | Component Cost | Battery Life | Replacement Cost (5 years) | Total Cost |
|-----------|----------------|--------------|----------------------------|------------|
| RV-3028-C7 | $6-8 | 10+ years | $0 | **$6-8** |
| AB1805 | $8-10 | 10+ years | $0 | **$8-10** |
| DS3231 | $4-6 | 2.4 years | $10-15 (2x replacement) | **$14-21** |

**Winner:** RV-3028-C7 (lowest total cost of ownership)

---

## 7. Final Recommendation

### 7.1 Recommended RTC: RV-3028-C7

**Reasons:**
1. **Ultra-low power:** 45 nA (10+ year battery life)
2. **Highest accuracy:** ±1 ppm with automatic temperature compensation
3. **Integrated crystal:** Simplifies hardware design, reduces cost
4. **Wide voltage range:** 1.1V - 5.5V (flexible power supply)
5. **Excellent breakout boards:** Adafruit, SparkFun (easy prototyping)
6. **Lowest total cost of ownership:** No battery replacements needed
7. **Small footprint:** 3.2 x 1.5 mm (compact design)
8. **Well-documented:** Good library support, examples available

### 7.2 Recommended Breakout Board

**Adafruit RV-3028 Real Time Clock Breakout (Product ID: 5188)**
- Price: ~$6.95
- Features:
  - RV-3028-C7 RTC with integrated crystal
  - I2C pull-up resistors included (check schematic)
  - Qwiic/STEMMA QT connectors (easy wiring)
  - CR1220 battery holder
  - 3.3V and 5V compatible
  - Compact size: 25.4 x 17.8 mm

**Alternative:** SparkFun Real Time Clock Module - RV-3028 (BOB-16281)

### 7.3 Procurement Recommendations

**For Development/Prototyping:**
- Quantity: 3-5 breakout boards
- Supplier: Adafruit, SparkFun, Mouser, Digikey
- Lead time: 1-2 weeks (stock availability)

**For Production:**
- Use bare RV-3028-C7 IC on custom PCB
- Cost savings: ~$4-5 per unit
- Requires custom PCB design

---

## 8. Alternative Scenarios

### 8.1 If RV-3028-C7 is Unavailable

**Fallback Option 1:** DS3231
- Pros: Widely available, lower cost, integrated crystal
- Cons: Higher power consumption (840 nA), shorter battery life
- Use case: Acceptable if battery replacement every 2-3 years is feasible

**Fallback Option 2:** AB1805
- Pros: Lowest power (22 nA), dual alarms
- Cons: Requires external crystal, higher complexity
- Use case: Only if ultra-low power is critical and crystal design expertise available

### 8.2 If Cost is Primary Concern

**Option:** DS3231
- Lower initial cost ($4-6 vs. $6-8)
- Trade-off: Higher battery replacement cost over 5 years
- Total cost of ownership: Higher ($14-21 vs. $6-8)

**Recommendation:** Still use RV-3028-C7 for lowest total cost

---

## 9. Conclusions

**Selected RTC:** RV-3028-C7

**Key Benefits:**
- Meets all technical requirements (accuracy, temperature range, power)
- Simplest hardware integration (integrated crystal)
- Lowest total cost of ownership (no battery replacements)
- Excellent availability and support

**Next Steps:**
1. Procure Adafruit RV-3028 breakout boards (3-5 units)
2. Verify I2C pull-up resistors on breakout board
3. Prepare electrical connection plan
4. Proceed to firmware driver development (Milestone 2)

---

**Document Status:** Complete  
**Review Status:** Pending client review  
**Next Document:** Electrical Connection Plan (03_electrical_connection_plan.md)

