# Milestone 1: Bill of Materials (BOM)

**Developer:** Abdul Raheem Ansari
**Email:** ansarirahim1@gmail.com | **WhatsApp:** +91 9024304883
**LinkedIn:** [linkedin.com/in/abdul-raheem-ansari-a6871320](https://www.linkedin.com/in/abdul-raheem-ansari-a6871320/)

---

## Complete Component List with Suppliers and Costs

**Project:** ESP-NOW RTC Synchronization System
**Date:** 2024-11-18
**Version:** 1.0.0-M1

---

## 1. Development/Prototyping BOM

### 1.1 Main Components

| Item | Part Number | Description | Qty (Sensor) | Qty (Gateway) | Unit Cost | Supplier | Link |
|------|-------------|-------------|--------------|---------------|-----------|----------|------|
| ESP32-S3 Dev Board | ESP32-S3-DevKitC-1 | ESP32-S3 development board | 1 | 1 | $10-15 | Adafruit, Mouser, Digikey | [Adafruit #5364](https://www.adafruit.com/product/5364) |
| RTC Module | RV-3028-C7 Breakout | Real-time clock breakout board | 1 | 1 | $6-8 | Adafruit, SparkFun | [Adafruit #5188](https://www.adafruit.com/product/5188) |
| Battery Holder | - | 2x AA battery holder with leads | 1 | 1 | $1-2 | Adafruit, Amazon | [Adafruit #3859](https://www.adafruit.com/product/3859) |
| Batteries | - | 2x AA Alkaline or Lithium | 2 | 2 | $2-10 | Local store, Amazon | - |
| Tactile Button | - | 6x6mm tactile push button | 0 | 1 | $0.10-0.25 | Adafruit, Amazon | [Adafruit #367](https://www.adafruit.com/product/367) |

**Subtotal (Sensor Node):** $19-27  
**Subtotal (Gateway Node):** $19-28

### 1.2 Passive Components (if not using breakout board)

| Item | Value | Package | Qty (Sensor) | Qty (Gateway) | Unit Cost | Supplier |
|------|-------|---------|--------------|---------------|-----------|----------|
| Resistor | 4.7 kΩ, ±5% | 0805 SMD or TH | 2 | 2 | $0.05 | Mouser, Digikey |
| Resistor | 10 kΩ, ±5% | 0805 SMD or TH | 1 | 2 | $0.05 | Mouser, Digikey |
| Capacitor | 100 nF, 10V, X7R | 0805 SMD or TH | 1 | 1 | $0.05 | Mouser, Digikey |
| Capacitor | 10 µF, 10V | Electrolytic | 1 | 1 | $0.10 | Mouser, Digikey |

**Note:** Adafruit RV-3028 breakout includes pull-up resistors. External resistors may not be needed.

**Subtotal (Passive Components):** $0.30-0.50 per node

### 1.3 Wiring and Connectors

| Item | Description | Qty | Unit Cost | Supplier |
|------|-------------|-----|-----------|----------|
| Jumper Wires | Male-to-female, 20cm, assorted colors | 1 pack (40 wires) | $3-5 | Adafruit, Amazon |
| Breadboard | 830-point solderless breadboard | 1 | $5-8 | Adafruit, Amazon |
| USB Cable | USB-C to USB-A (for ESP32-S3) | 1 | $3-5 | Amazon |

**Subtotal (Wiring/Tools):** $11-18 (one-time purchase)

### 1.4 Total Development Cost

**For 1 Sensor Node + 1 Gateway Node:**
- Main components: $38-55
- Passive components: $0.60-1.00
- Wiring/tools: $11-18 (one-time)
- **Total:** $50-74

**For 3 Sensor Nodes + 1 Gateway Node (recommended for testing):**
- Main components: $76-109
- Passive components: $1.20-2.00
- Wiring/tools: $11-18 (one-time)
- **Total:** $88-129

---

## 2. Production BOM (Custom PCB)

### 2.1 Main Components

| Item | Part Number | Description | Qty per Node | Unit Cost (1-10) | Unit Cost (100+) | Supplier |
|------|-------------|-------------|--------------|------------------|------------------|----------|
| ESP32-S3 Module | ESP32-S3-WROOM-1-N8R8 | ESP32-S3 module, 8MB flash, 8MB PSRAM | 1 | $3.50-4.50 | $2.50-3.00 | Mouser, Digikey |
| RTC IC | RV-3028-C7 | Real-time clock IC, 3.2x1.5mm | 1 | $2.00-3.00 | $1.50-2.00 | Mouser, Digikey |
| Voltage Regulator | AP2112K-3.3 | 3.3V LDO, 600mA, SOT-23-5 | 1 | $0.30-0.50 | $0.20-0.30 | Mouser, Digikey |
| Battery Holder | - | 2x AA battery holder, SMD | 1 | $0.50-1.00 | $0.30-0.50 | Mouser, Digikey |
| Tactile Button | - | 6x6mm tactile switch, SMD (Gateway only) | 0/1 | $0.10-0.20 | $0.05-0.10 | Mouser, Digikey |

### 2.2 Passive Components

| Item | Value | Package | Qty (Sensor) | Qty (Gateway) | Unit Cost (100+) |
|------|-------|---------|--------------|---------------|------------------|
| Resistor | 4.7 kΩ, ±5%, 1/8W | 0805 SMD | 2 | 2 | $0.01 |
| Resistor | 10 kΩ, ±5%, 1/8W | 0805 SMD | 1 | 2 | $0.01 |
| Resistor | 10 kΩ, ±5%, 1/8W (pull-down) | 0805 SMD | 0 | 1 | $0.01 |
| Capacitor | 100 nF, 10V, X7R | 0805 SMD | 2 | 2 | $0.02 |
| Capacitor | 10 µF, 10V, X7R | 0805 SMD | 2 | 2 | $0.05 |
| Capacitor | 1 µF, 10V, X7R | 0805 SMD | 1 | 1 | $0.03 |

### 2.3 PCB and Assembly

| Item | Description | Unit Cost (1-10) | Unit Cost (100+) |
|------|-------------|------------------|------------------|
| PCB | 2-layer, 50x40mm, ENIG finish | $2-5 | $0.50-1.00 |
| Assembly | SMD assembly (pick-and-place) | $5-10 | $1-2 |
| Stencil | Solder paste stencil (one-time) | $15-25 | - |

### 2.4 Total Production Cost per Node

**Sensor Node (100+ units):**
- ESP32-S3 module: $2.50-3.00
- RTC IC: $1.50-2.00
- Voltage regulator: $0.20-0.30
- Passive components: $0.20-0.30
- PCB: $0.50-1.00
- Assembly: $1-2
- **Total:** $6-9 per node

**Gateway Node (100+ units):**
- Same as sensor + button: $0.05-0.10
- **Total:** $6-9 per node

**Cost Savings vs. Development:** ~$13-18 per node (68-75% reduction)

---

## 3. Detailed Component Specifications

### 3.1 ESP32-S3 Development Board

**Adafruit ESP32-S3 Feather (Product #5364)**
- ESP32-S3 dual-core @ 240 MHz
- 8 MB flash, 2 MB PSRAM
- WiFi 802.11 b/g/n, Bluetooth 5 LE
- USB-C connector
- LiPo battery connector and charger
- 3.3V regulator
- Dimensions: 50.8 x 22.9 mm
- **Price:** $12.95

**Alternative:** ESP32-S3-DevKitC-1 (Espressif official, $10-12)

### 3.2 RTC Module

**Adafruit RV-3028 Real Time Clock Breakout (Product #5188)**
- RV-3028-C7 RTC with integrated crystal
- I2C interface (address 0x52)
- Alarm with interrupt output
- Battery backup (CR1220 holder included)
- Qwiic/STEMMA QT connectors
- 10kΩ pull-up resistors on SDA, SCL, INT
- Dimensions: 25.4 x 17.8 mm
- **Price:** $6.95

**Alternative:** SparkFun RTC Module - RV-3028 (BOB-16281, $7.95)

### 3.3 Batteries

**Alkaline (General Purpose):**
- Energizer AA Alkaline (2-pack)
- Capacity: 3000 mAh @ 1.5V
- Operating temp: -18°C to +55°C
- Shelf life: 5-7 years
- **Price:** $2-3 per 2-pack

**Lithium (Cold Environment):**
- Energizer AA Lithium (2-pack)
- Capacity: 3500 mAh @ 1.5V
- Operating temp: -40°C to +60°C
- Shelf life: 10-15 years
- Better cold performance
- **Price:** $8-10 per 2-pack

**Recommendation:** Use lithium for sensor nodes in cold environments (-5°C to +8°C)

---

## 4. Supplier Information

### 4.1 Primary Suppliers

**Adafruit Industries**
- Website: https://www.adafruit.com
- Shipping: Worldwide
- Lead time: 1-2 weeks (US), 2-4 weeks (international)
- Minimum order: None
- Payment: Credit card, PayPal

**SparkFun Electronics**
- Website: https://www.sparkfun.com
- Shipping: Worldwide
- Lead time: 1-2 weeks (US), 2-4 weeks (international)
- Minimum order: None
- Payment: Credit card, PayPal

**Mouser Electronics**
- Website: https://www.mouser.com
- Shipping: Worldwide
- Lead time: 1-3 days (US), 1-2 weeks (international)
- Minimum order: None
- Payment: Credit card, PayPal, wire transfer

**Digikey**
- Website: https://www.digikey.com
- Shipping: Worldwide
- Lead time: 1-3 days (US), 1-2 weeks (international)
- Minimum order: None
- Payment: Credit card, PayPal, wire transfer

### 4.2 Local Suppliers (Turkey)

**Direnc.net**
- Website: https://www.direnc.net
- ESP32 boards, components, sensors
- Local shipping (1-3 days)

**Robotistan**
- Website: https://www.robotistan.com
- ESP32 boards, RTC modules, components
- Local shipping (1-3 days)

---

## 5. Procurement Recommendations

### 5.1 For Milestone 1 (Development)

**Immediate Purchase:**
- 3x ESP32-S3 development boards (Adafruit or Espressif)
- 3x RV-3028 RTC breakout boards (Adafruit or SparkFun)
- 3x 2-pack AA lithium batteries (for cold environment testing)
- 1x Breadboard and jumper wires (if not already available)
- 1x Tactile button (for gateway)

**Total Cost:** $88-129

**Lead Time:** 1-2 weeks (US), 2-4 weeks (international)

### 5.2 For Milestone 2 (Firmware Development)

**No additional hardware required** (use Milestone 1 hardware)

### 5.3 For Milestone 3 (Testing)

**Additional Items:**
- 1x USB power meter (for current measurement)
- 1x Multimeter (if not already available)
- Optional: Temperature chamber or refrigerator (for cold testing)

**Total Cost:** $20-50

---

## 6. Cost Summary

### 6.1 Development Phase (Milestones 1-3)

| Item | Quantity | Unit Cost | Total Cost |
|------|----------|-----------|------------|
| ESP32-S3 boards | 3 | $12 | $36 |
| RTC modules | 3 | $7 | $21 |
| Batteries (lithium) | 3 packs | $9 | $27 |
| Button | 1 | $0.25 | $0.25 |
| Breadboard/wires | 1 set | $10 | $10 |
| USB power meter | 1 | $20 | $20 |
| **Total** | | | **$114** |

### 6.2 Production Phase (100+ units)

| Item | Quantity | Unit Cost | Total Cost |
|------|----------|-----------|------------|
| Sensor nodes | 100 | $7 | $700 |
| Gateway nodes | 10 | $7 | $70 |
| **Total** | 110 | | **$770** |

**Cost per Node:** $7 (vs. $25-30 for development boards)

---

## 7. Alternatives and Substitutions

### 7.1 ESP32-S3 Alternatives

**ESP32-C3:**
- Lower cost ($8-10 vs. $12-15)
- Single-core RISC-V (vs. dual-core Xtensa)
- Sufficient for sensor nodes
- Same WiFi/ESP-NOW capabilities

**ESP32-S2:**
- Similar cost to ESP32-S3
- Single-core Xtensa
- No Bluetooth (not needed for this project)

**Recommendation:** Use ESP32-S3 for gateway (more processing power), ESP32-C3 for sensor nodes (cost savings)

### 7.2 RTC Alternatives

**DS3231:**
- Lower cost ($4-6 vs. $6-8)
- Higher power consumption (840 nA vs. 45 nA)
- Shorter battery life (2-3 years vs. 10+ years)
- Use only if cost is critical and battery replacement is acceptable

**AB1805:**
- Higher cost ($8-10 vs. $6-8)
- Requires external crystal (added complexity)
- Lowest power (22 nA vs. 45 nA)
- Use only if ultra-low power is critical

**Recommendation:** Stick with RV-3028-C7 (best balance)

---

## 8. Inventory Management

### 8.1 Recommended Stock Levels (Development)

| Item | Minimum Stock | Reorder Point | Lead Time |
|------|---------------|---------------|-----------|
| ESP32-S3 boards | 3 | 1 | 1-2 weeks |
| RTC modules | 3 | 1 | 1-2 weeks |
| Batteries | 6 (3 packs) | 2 (1 pack) | 1 week |
| Passive components | 10 each | 5 each | 1-2 weeks |

### 8.2 Recommended Stock Levels (Production)

| Item | Minimum Stock | Reorder Point | Lead Time |
|------|---------------|---------------|-----------|
| ESP32-S3 modules | 50 | 20 | 2-4 weeks |
| RTC ICs | 50 | 20 | 2-4 weeks |
| PCBs | 50 | 20 | 2-3 weeks |
| Passive components | 200 each | 50 each | 1-2 weeks |

---

## 9. Conclusions

### 9.1 Development Cost

**Total hardware cost for Milestones 1-3:** $114

**Breakdown:**
- 3x sensor nodes: $75
- 1x gateway node: $25
- Test equipment: $14

### 9.2 Production Cost

**Cost per node (100+ units):** $7

**Cost savings:** 68-75% vs. development boards

### 9.3 Procurement Timeline

**Week 1:** Order components from Adafruit/SparkFun  
**Week 2-3:** Receive components, begin Milestone 1  
**Week 4-5:** Complete Milestones 1-2  
**Week 6:** Complete Milestone 3, deliver final report

---

**Document Status:** Complete  
**Review Status:** Pending client review  
**Next Milestone:** Firmware Implementation (Milestone 2)

