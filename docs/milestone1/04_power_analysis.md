# Milestone 1: Power Analysis and Battery Life Projection

## Detailed Power Consumption Analysis

**Project:** ESP-NOW RTC Synchronization System  
**Date:** 2024-11-18  
**Author:** Abdul Raheem Ansari  
**Version:** 1.0.0-M1

---

## 1. Executive Summary

**Key Findings:**
- **Deep Sleep Current:** 7-12 µA (ESP32-S3 + RTC + resistors)
- **Active Current (ESP-NOW):** 100-200 mA (brief bursts)
- **Battery Life (1-hour interval):** 2-3 years on 2x AA batteries (3000 mAh)
- **Battery Life (10-min interval):** 6-12 months on 2x AA batteries
- **RTC Contribution:** <1% of total power consumption (negligible)

---

## 2. Component Power Consumption

### 2.1 ESP32-S3 Power Modes

| Mode | Current | Duration | Notes |
|------|---------|----------|-------|
| **Deep Sleep** | 5-10 µA | 99.9% of time | RTC timer + ULP active |
| **Wake-up** | 40-80 mA | ~100 ms | Boot, init peripherals |
| **WiFi Init** | 80-120 mA | ~200 ms | WiFi/ESP-NOW initialization |
| **ESP-NOW TX** | 150-200 mA | ~50 ms | Transmit packet |
| **ESP-NOW RX** | 80-120 mA | ~100 ms | Receive packet |
| **I2C Communication** | 40-60 mA | ~10 ms | Read/write RTC |

**Typical Wake Cycle (Sensor Node):**
1. Wake from deep sleep: 100 ms @ 60 mA
2. Read RTC time: 10 ms @ 50 mA
3. ESP-NOW init: 200 ms @ 100 mA
4. Transmit data: 50 ms @ 180 mA
5. Receive ACK: 100 ms @ 100 mA
6. Set next alarm: 10 ms @ 50 mA
7. Return to deep sleep: 50 ms @ 40 mA

**Total Active Time:** ~520 ms  
**Average Active Current:** ~100 mA

### 2.2 RV-3028-C7 RTC Power Consumption

| Mode | Current | Duration | Notes |
|------|---------|----------|-------|
| **Timekeeping** | 45 nA | 99.9% of time | Crystal oscillator running |
| **I2C Communication** | 100 µA | ~10 ms per wake | Reading/writing registers |
| **Alarm Active** | 45 nA | Continuous | No additional power |

**Average Current:** ~45 nA (I2C bursts negligible)

### 2.3 Passive Components Power Consumption

**Pull-up Resistors:**
- SDA pull-up (4.7kΩ): 0.7 mA when LOW (during I2C communication)
- SCL pull-up (4.7kΩ): 0.7 mA when LOW (during I2C communication)
- INT pull-up (10kΩ): 0.33 mA when LOW (during alarm, brief)

**Average Current (Deep Sleep):**
- SDA/SCL: ~0 µA (lines idle HIGH)
- INT: ~0 µA (line idle HIGH)

**Average Current (Active):**
- SDA/SCL: ~1-2 µA (averaged over wake cycle)
- INT: ~0 µA (alarm duration negligible)

**Total Resistor Current:** ~2 µA average

---

## 3. Power Budget Analysis

### 3.1 Deep Sleep Mode (99.9% of time)

| Component | Current | Percentage |
|-----------|---------|------------|
| ESP32-S3 | 5-10 µA | 83-91% |
| RV-3028-C7 RTC | 0.045 µA | <1% |
| Pull-up resistors | ~2 µA | 17-18% |
| **Total** | **7-12 µA** | **100%** |

**Key Insight:** ESP32-S3 deep sleep current dominates. RTC and resistors contribute <20%.

### 3.2 Active Mode (0.1% of time)

| Component | Current | Percentage |
|-----------|---------|------------|
| ESP32-S3 (WiFi/ESP-NOW) | 100-200 mA | 99.9% |
| RV-3028-C7 (I2C) | 0.1 mA | <0.1% |
| Pull-up resistors | 1-2 mA | <1% |
| **Total** | **100-200 mA** | **100%** |

**Key Insight:** ESP32-S3 WiFi/ESP-NOW dominates active power. RTC and resistors negligible.

---

## 4. Duty Cycle Analysis

### 4.1 Sensor Node Operation

**Scenario 1: 1-Hour Wake Interval**
- Sleep duration: 3600 seconds
- Active duration: 0.52 seconds (520 ms)
- Duty cycle: 0.52 / 3600 = 0.014% (0.00014)

**Scenario 2: 10-Minute Wake Interval**
- Sleep duration: 600 seconds
- Active duration: 0.52 seconds
- Duty cycle: 0.52 / 600 = 0.087% (0.00087)

**Scenario 3: 10-Second Measurement (1-Hour Transmission)**
- Measurement: Every 10 seconds (sensor reading only, no WiFi)
- Transmission: Every 3600 seconds (ESP-NOW transmission)
- Measurement active time: 50 ms @ 40 mA
- Transmission active time: 520 ms @ 100 mA

### 4.2 Gateway Node Operation

**Scenario: 1-Hour Wake Interval**
- Sleep duration: 3600 seconds
- Active duration: 1.0 second (receive from multiple sensors)
- Duty cycle: 1.0 / 3600 = 0.028% (0.00028)

**Manual Wake Events:**
- Frequency: Assume 5 times per day
- Duration: 30 seconds per wake (GUI display)
- Additional active time: 5 × 30 = 150 seconds per day
- Daily duty cycle increase: 150 / 86400 = 0.17%

---

## 5. Average Current Calculation

### 5.1 Sensor Node (1-Hour Interval)

**Formula:**
```
I_avg = (I_sleep × T_sleep + I_active × T_active) / (T_sleep + T_active)
```

**Calculation:**
```
I_sleep = 10 µA = 0.01 mA
T_sleep = 3600 s
I_active = 100 mA
T_active = 0.52 s

I_avg = (0.01 mA × 3600 s + 100 mA × 0.52 s) / (3600 s + 0.52 s)
I_avg = (36 mA·s + 52 mA·s) / 3600.52 s
I_avg = 88 mA·s / 3600.52 s
I_avg ≈ 0.024 mA = 24 µA
```

**Average Current:** ~24 µA

### 5.2 Sensor Node (10-Minute Interval)

**Calculation:**
```
I_sleep = 10 µA = 0.01 mA
T_sleep = 600 s
I_active = 100 mA
T_active = 0.52 s

I_avg = (0.01 mA × 600 s + 100 mA × 0.52 s) / (600 s + 0.52 s)
I_avg = (6 mA·s + 52 mA·s) / 600.52 s
I_avg = 58 mA·s / 600.52 s
I_avg ≈ 0.097 mA = 97 µA
```

**Average Current:** ~97 µA

### 5.3 Sensor Node (10-Second Measurement, 1-Hour Transmission)

**Measurement Cycle:**
```
I_sleep = 10 µA = 0.01 mA
T_sleep = 10 s
I_measure = 40 mA
T_measure = 0.05 s (50 ms)

I_avg_measure = (0.01 mA × 10 s + 40 mA × 0.05 s) / 10.05 s
I_avg_measure = (0.1 mA·s + 2 mA·s) / 10.05 s
I_avg_measure ≈ 0.21 mA = 210 µA
```

**Transmission Cycle (once per hour):**
```
Additional current per hour = 100 mA × 0.52 s / 3600 s ≈ 0.014 mA = 14 µA
```

**Total Average Current:** 210 µA + 14 µA = **224 µA**

### 5.4 Gateway Node (1-Hour Interval + Manual Wakes)

**Scheduled Wake Cycle:**
```
I_avg_scheduled = 24 µA (same as sensor node)
```

**Manual Wake Cycles (5 per day, 30 seconds each):**
```
I_manual = 80 mA (GUI display, no WiFi)
T_manual_per_day = 5 × 30 s = 150 s
T_day = 86400 s

I_avg_manual = (80 mA × 150 s) / 86400 s ≈ 0.139 mA = 139 µA
```

**Total Average Current:** 24 µA + 139 µA = **163 µA**

---

## 6. Battery Life Projections

### 6.1 Battery Options

| Battery Type | Voltage | Capacity | Cost | Notes |
|--------------|---------|----------|------|-------|
| 2x AA Alkaline | 3.0V | 3000 mAh | $2-3 | Common, good capacity |
| 2x AA Lithium | 3.0V | 3500 mAh | $8-10 | Better cold performance |
| 18650 Li-ion | 3.7V | 3000 mAh | $5-8 | Rechargeable, needs regulator |
| CR123A Lithium | 3.0V | 1500 mAh | $3-5 | Compact, good cold performance |

**Recommended:** 2x AA Alkaline (good balance of cost, capacity, availability)

### 6.2 Battery Life Calculations

**Formula:**
```
Battery Life (hours) = (Battery Capacity × Efficiency) / Average Current
Battery Life (days) = Battery Life (hours) / 24
Battery Life (years) = Battery Life (days) / 365
```

**Assumptions:**
- Battery capacity: 3000 mAh (2x AA Alkaline)
- Efficiency: 80% (account for self-discharge, temperature effects)
- Effective capacity: 3000 mAh × 0.8 = 2400 mAh

### 6.3 Sensor Node Battery Life

**Scenario 1: 1-Hour Interval**
```
I_avg = 24 µA = 0.024 mA
Battery Life = 2400 mAh / 0.024 mA = 100,000 hours
Battery Life = 100,000 / 24 = 4,167 days
Battery Life ≈ 11.4 years
```

**Practical Limit:** Battery shelf life (~5-7 years for alkaline)

**Scenario 2: 10-Minute Interval**
```
I_avg = 97 µA = 0.097 mA
Battery Life = 2400 mAh / 0.097 mA = 24,742 hours
Battery Life = 24,742 / 24 = 1,031 days
Battery Life ≈ 2.8 years
```

**Scenario 3: 10-Second Measurement, 1-Hour Transmission**
```
I_avg = 224 µA = 0.224 mA
Battery Life = 2400 mAh / 0.224 mA = 10,714 hours
Battery Life = 10,714 / 24 = 446 days
Battery Life ≈ 1.2 years
```

### 6.4 Gateway Node Battery Life

**1-Hour Interval + Manual Wakes (5/day)**
```
I_avg = 163 µA = 0.163 mA
Battery Life = 2400 mAh / 0.163 mA = 14,724 hours
Battery Life = 14,724 / 24 = 613 days
Battery Life ≈ 1.7 years
```

**Note:** Gateway typically powered by mains (USB or wall adapter). Battery backup optional.

---

## 7. Battery Life Summary Table

| Configuration | Average Current | Battery Life (2x AA, 3000 mAh) |
|---------------|-----------------|--------------------------------|
| Sensor: 1-hour interval | 24 µA | **11.4 years** (limited by shelf life) |
| Sensor: 10-min interval | 97 µA | **2.8 years** |
| Sensor: 10s measure, 1h TX | 224 µA | **1.2 years** |
| Gateway: 1-hour + manual | 163 µA | **1.7 years** |

---

## 8. Power Optimization Recommendations

### 8.1 Firmware Optimizations

1. **Minimize Wake Time:**
   - Pre-calculate next alarm time
   - Use fast boot mode (skip bootloader delay)
   - Optimize ESP-NOW packet size

2. **Reduce WiFi Power:**
   - Use ESP-NOW instead of WiFi (lower power)
   - Minimize packet retransmissions (good signal strength)
   - Use power-save mode during active periods

3. **Optimize I2C Communication:**
   - Use 100 kHz I2C (lower power than 400 kHz)
   - Minimize register reads/writes
   - Batch RTC operations

### 8.2 Hardware Optimizations

1. **Use Lithium Batteries for Cold Environments:**
   - Better performance at -5°C to +8°C
   - Higher capacity retention

2. **Add Battery Backup to RTC:**
   - CR2032 coin cell (10+ year life)
   - Maintains time even when main battery is replaced

3. **Use Low-Power Voltage Regulator:**
   - Quiescent current <10 µA
   - High efficiency at low currents

---

## 9. Temperature Effects on Battery Life

### 9.1 Alkaline Battery Performance

| Temperature | Capacity Retention | Effective Capacity (3000 mAh) |
|-------------|-------------------|-------------------------------|
| +20°C | 100% | 3000 mAh |
| +8°C | 90-95% | 2700-2850 mAh |
| -5°C | 60-70% | 1800-2100 mAh |

**Impact on Battery Life (Sensor at -5°C):**
```
Scenario: 10s measure, 1h TX
I_avg = 224 µA
Effective capacity at -5°C = 1800 mAh × 0.8 = 1440 mAh
Battery Life = 1440 mAh / 0.224 mA = 6,429 hours ≈ 268 days ≈ 0.7 years
```

**Recommendation:** Use lithium batteries for cold environments (better cold performance).

### 9.2 Lithium Battery Performance

| Temperature | Capacity Retention | Effective Capacity (3500 mAh) |
|-------------|-------------------|-------------------------------|
| +20°C | 100% | 3500 mAh |
| +8°C | 95-100% | 3325-3500 mAh |
| -5°C | 90-95% | 3150-3325 mAh |

**Impact on Battery Life (Sensor at -5°C):**
```
Effective capacity at -5°C = 3150 mAh × 0.8 = 2520 mAh
Battery Life = 2520 mAh / 0.224 mA = 11,250 hours ≈ 469 days ≈ 1.3 years
```

**Improvement:** 1.3 years vs. 0.7 years (86% increase)

---

## 10. Conclusions

### 10.1 Key Findings

1. **Deep Sleep Dominates:** System spends 99.9% of time in deep sleep
2. **RTC Negligible:** RTC adds <1% to power consumption
3. **Battery Life Excellent:** 1-3 years on 2x AA batteries (depending on interval)
4. **Temperature Matters:** Use lithium batteries for cold environments

### 10.2 Recommendations

1. **For 1-Hour Interval:** Use alkaline batteries (11+ year life, limited by shelf life)
2. **For 10-Minute Interval:** Use alkaline batteries (2.8 year life)
3. **For 10-Second Measurement:** Use lithium batteries (1.2-1.3 year life)
4. **For Cold Environments (-5°C):** Use lithium batteries (better capacity retention)
5. **For Gateway:** Use mains power (USB/wall adapter) with battery backup

### 10.3 Next Steps

1. Validate power measurements with actual hardware (Milestone 3)
2. Optimize firmware for minimum wake time
3. Test battery life in cold environment (-5°C)

---

**Document Status:** Complete  
**Review Status:** Pending client review  
**Next Document:** Bill of Materials (05_bom.md)

