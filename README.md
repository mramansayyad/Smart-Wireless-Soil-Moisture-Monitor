# 🌱 Smart Wireless Soil Moisture Monitor

![Banner](/images/main.png)

A high-performance IoT solution for real-time soil health monitoring, featuring a premium **Apple-inspired Glassmorphism UI**. This project transforms a NodeMCU ESP8266 into a standalone web server that provides instant agricultural insights without requiring an internet connection.

---

## 🚀 Overview

This system leverages the **ESP8266** to create a localized Wi-Fi Access Point. It samples soil moisture data through an analog sensor, processes the signals to determine soil state, and serves a modern, responsive dashboard to any connected device.

### Key Features
- **Standalone Ecosystem:** Operates as an independent Access Point (AP Mode).
- **Premium UI/UX:** Responsive Apple-style dashboard with blur-effects and live gauge animations.
- **Real-time Data:** 2-second refresh interval using asynchronous JSON fetching.
- **Diagnostics:** Built-in "Sensor Health" monitoring to detect hardware faults or disconnections.
- **Calibration Ready:** Easily adjustable dry/wet thresholds for different soil types.

---

## 🛠️ Hardware Requirements

| Component | Function |
| :--- | :--- |
| **NodeMCU ESP8266** | Core Microcontroller & Wi-Fi Server |
| **Soil Moisture Sensor** | Analog water content detection |
| **Jumper Wires** | Circuit interconnections |
| **Power Source** | 5V Micro-USB or Battery pack |

### Circuit Diagram
![Circuitry](/images/circuit.png)

---

## 💻 Technical Architecture

### 1. The Backend (C++/Arduino)
The firmware is built on the `ESP8266WebServer` library. It handles two primary routes:
- `/` : Serves the minified HTML/CSS/JS stored in Program Memory (`PROGMEM`).
- `/data` : Returns a JSON object containing `rawADC`, `moisture%`, `uptime`, and `status`.

### 2. The Frontend (HTML5/CSS3/JS)
The UI utilizes modern CSS features:
- **Glassmorphism:** `backdrop-filter: blur(15px)` and semi-transparent white backgrounds.
- **Dynamic Gauges:** `conic-gradient` updated via JavaScript DOM manipulation.
- **Responsiveness:** CSS Grid and Flexbox ensure compatibility from iPhones to Laptops.

---

## ⚙️ Installation & Setup

1. **Hardware Setup:**
   - Connect Sensor **VCC** to **3V3**.
   - Connect Sensor **GND** to **GND**.
   - Connect Sensor **Analog Out** to **A0**.

3. **Software Setup:**
   - Open `soil_monitor.ino` in Arduino IDE.
   - Install the **ESP8266 Board Package**.
   - Select `NodeMCU 1.0 (ESP-12E Module)` as the board.
   - Click **Upload**.

4. **Connecting:**
   - Scan for Wi-Fi: `Smart_Soil_Monitor`
   - Password: `12345678`
   - Open Browser: `http://192.168.4.1`

---

## 📊 Performance Indicators
- **Dry Soil:** 0% - 35% (High resistance)
- **Normal:** 36% - 70% (Optimal moisture)
- **Wet Soil:** 71% - 100% (Low resistance)

---

## 👤 Author
**Aman Sayyad** *Core Developer & Embedded Systems Engineer*
