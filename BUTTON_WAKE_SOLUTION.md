# Button Wake-Up Solution — Maintaining Sync During Manual Wake Events

## Problem Statement

**Critical Requirement:**
- Gateway has a physical button that user can press at any time to wake device and show GUI
- Manual wake-up must NOT interfere with scheduled sleep cycle
- Manual wake-up must NOT shift or delay next server transmission timestamp
- System must maintain synchronization with sensor nodes

**Current System Behavior:**
- Sensor nodes: Deep sleep, wake every 10 seconds to measure, transmit to gateway once per hour
- Gateway: Deep sleep, wakes at configured interval (1 hour default, configurable to 10/30 min) to receive data and forward to server
- One sensor operates between -5°C to +8°C

---

## Solution: Dual Wake Source with Absolute Time Scheduling

### Key Concept

Use **absolute timestamps** instead of relative sleep durations. The RTC alarm is always set to the **next scheduled transmission time**, regardless of manual wake events.

### Implementation Strategy

#### 1. Two Wake Sources

Configure ESP32 to wake from **two independent sources**:

```c
// Wake Source 1: RTC alarm (scheduled transmission)
esp_sleep_enable_ext0_wakeup(GPIO_RTC_INT, 0); // RTC INT pin

// Wake Source 2: Button press (manual wake for GUI)
esp_sleep_enable_ext1_wakeup(GPIO_BUTTON_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
```

#### 2. Absolute Time Scheduling

**Instead of:**
```c
// WRONG: Relative sleep duration
rtc_set_alarm_relative(3600); // Sleep for 1 hour
esp_deep_sleep_start();
```

**Use:**
```c
// CORRECT: Absolute timestamp scheduling
uint32_t next_transmission_time = get_next_transmission_timestamp();
rtc_set_alarm_absolute(next_transmission_time);
esp_deep_sleep_start();
```

#### 3. Wake Reason Detection

```c
void gateway_main_loop() {
    esp_sleep_wakeup_cause_t wake_cause = esp_sleep_get_wakeup_cause();
    
    if (wake_cause == ESP_SLEEP_WAKEUP_EXT0) {
        // Woke from RTC alarm (scheduled transmission)
        handle_scheduled_transmission();
        
    } else if (wake_cause == ESP_SLEEP_WAKEUP_EXT1) {
        // Woke from button press (manual GUI wake)
        handle_manual_wake();
    }
    
    // Always set alarm to SAME next transmission time
    rtc_set_alarm_absolute(next_transmission_time);
    esp_deep_sleep_start();
}
```

---

## Detailed Implementation

### Gateway Firmware Logic

```c
// Global: Next scheduled transmission timestamp (Unix time)
static uint32_t g_next_transmission_timestamp = 0;

void gateway_init() {
    // Initialize RTC
    rtc_init();
    
    // Read current time from RTC
    uint32_t current_time = rtc_get_unix_time();
    
    // Calculate next transmission time (aligned to hour boundary)
    // Example: If interval is 3600 seconds (1 hour)
    uint32_t interval = 3600; // 1 hour (configurable: 600, 1800, 3600)
    g_next_transmission_timestamp = ((current_time / interval) + 1) * interval;
    
    // Set RTC alarm to next transmission time
    rtc_set_alarm_absolute(g_next_transmission_timestamp);
}

void handle_scheduled_transmission() {
    // This is the scheduled hourly wake-up
    
    // Initialize ESP-NOW
    espnow_init();
    
    // Listen for sensor data (500 ms window)
    uint32_t start = millis();
    while (millis() - start < 500) {
        // Receive sensor packets
        // Store data
    }
    
    // Forward data to server (WiFi/LTE)
    send_data_to_server();
    
    // Shutdown radios
    espnow_deinit();
    wifi_deinit();
    
    // Calculate NEXT transmission time
    uint32_t interval = get_transmission_interval(); // 600, 1800, or 3600
    g_next_transmission_timestamp += interval;
    
    // Set alarm to next transmission (absolute time)
    rtc_set_alarm_absolute(g_next_transmission_timestamp);
}

void handle_manual_wake() {
    // User pressed button to view GUI
    
    // Show GUI (LCD/OLED display)
    display_init();
    show_current_data();
    
    // Wait for user interaction or timeout
    uint32_t start = millis();
    while (millis() - start < 30000) { // 30 second timeout
        if (button_pressed_again()) {
            // User interaction - extend timeout
            start = millis();
        }
        update_display();
    }
    
    // Shutdown display
    display_deinit();
    
    // DO NOT change g_next_transmission_timestamp
    // Alarm is already set to correct time - just go back to sleep
}

void gateway_main_loop() {
    esp_sleep_wakeup_cause_t wake_cause = esp_sleep_get_wakeup_cause();
    
    if (wake_cause == ESP_SLEEP_WAKEUP_EXT0) {
        // RTC alarm triggered (scheduled transmission)
        handle_scheduled_transmission();
        
    } else if (wake_cause == ESP_SLEEP_WAKEUP_EXT1) {
        // Button pressed (manual wake)
        handle_manual_wake();
        
    } else {
        // First boot or reset
        gateway_init();
    }
    
    // Enter deep sleep (alarm already set to next transmission time)
    esp_deep_sleep_start();
}
```

---

## Hardware Configuration

### Button Connection

```
Button Pin (GPIO4) ──┬── 10kΩ pull-down to GND
                     │
                     └── Button ── 3.3V
```

When button is pressed, GPIO4 goes HIGH, triggering wake-up.

### Wake Source Configuration

```c
// Configure wake sources
#define GPIO_RTC_INT    GPIO_NUM_3  // RTC alarm interrupt
#define GPIO_BUTTON     GPIO_NUM_4  // Manual wake button

void configure_wake_sources() {
    // Wake source 1: RTC alarm (EXT0)
    esp_sleep_enable_ext0_wakeup(GPIO_RTC_INT, 0); // Wake on LOW
    
    // Wake source 2: Button (EXT1)
    uint64_t button_mask = (1ULL << GPIO_BUTTON);
    esp_sleep_enable_ext1_wakeup(button_mask, ESP_EXT1_WAKEUP_ANY_HIGH);
}
```

---

## Synchronization Guarantee

### Why This Works

1. **Absolute Time Reference:**
   - Next transmission time is stored as absolute Unix timestamp
   - RTC alarm is set to absolute time, not relative duration
   - Manual wake events do NOT modify this timestamp

2. **RTC Maintains Time:**
   - RTC continues running during manual wake events
   - Alarm remains set to original transmission time
   - When returning to sleep, alarm is still active

3. **No Drift Accumulation:**
   - Each transmission time is calculated from fixed interval boundaries
   - Example: If interval is 1 hour, transmissions occur at :00, 1:00, 2:00, etc.
   - Manual wakes at :15, :37, etc. do not affect this schedule

### Example Timeline

```
Time    Event
-----   -----
00:00   Gateway wakes (scheduled), receives sensor data, transmits to server
00:00   Sets next alarm to 01:00 (absolute time)
00:00   Returns to deep sleep

00:15   User presses button (manual wake)
00:15   Gateway wakes, shows GUI
00:15   Alarm is still set to 01:00 (unchanged)
00:16   User done, gateway returns to sleep

01:00   Gateway wakes (scheduled), receives sensor data, transmits to server
01:00   Sets next alarm to 02:00 (absolute time)
01:00   Returns to deep sleep

01:42   User presses button (manual wake)
01:42   Gateway wakes, shows GUI
01:42   Alarm is still set to 02:00 (unchanged)
01:43   User done, gateway returns to sleep

02:00   Gateway wakes (scheduled), receives sensor data, transmits to server
        ... cycle continues ...
```

**Result:** Scheduled transmissions occur exactly at :00, 1:00, 2:00, etc., regardless of manual wake events.

---

## RTC Alarm Behavior During Manual Wake

### Important: RTC Alarm Persistence

When ESP32 wakes from button press:
- RTC alarm is **still active** and set to next transmission time
- If current time < alarm time: Alarm will trigger at scheduled time
- If current time >= alarm time: Alarm has already triggered (missed)

### Handling Missed Alarms

```c
void handle_manual_wake() {
    // Check if we missed a scheduled transmission
    uint32_t current_time = rtc_get_unix_time();
    
    if (current_time >= g_next_transmission_timestamp) {
        // We missed a scheduled transmission during manual wake
        // Handle it now
        handle_scheduled_transmission();
        return;
    }
    
    // Normal manual wake (no missed transmission)
    show_gui();
    
    // Return to sleep (alarm still set correctly)
}
```

---

## Summary

**Solution Components:**
1. Dual wake sources (RTC alarm + button)
2. Absolute time scheduling (not relative sleep durations)
3. Wake reason detection and appropriate handling
4. Missed alarm detection during manual wake

**Guarantees:**
- Scheduled transmissions occur at exact intervals (1 hour, 30 min, 10 min)
- Manual wake events do NOT shift transmission schedule
- System maintains synchronization with sensor nodes
- Works reliably in -5°C to +8°C temperature range

**Next Steps:**
- Implement in Milestone 2 firmware
- Test manual wake during scheduled transmission window
- Validate no drift over 24-hour period with random manual wakes

