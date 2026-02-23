# Gas Station Pump Firmware (ESP32)

Embedded firmware for a connected gas station pump controller built on ESP32.

## Overview

This project implements a state machine–based architecture to manage the full pump lifecycle, including:

- WiFi and WebSocket connectivity  
- Pump transaction states (Idle → Payment → Authorization → Fueling → Completion)  
- Motor control and actuator management  
- Sensor acquisition (flow, system status)  
- Local graphical interface using LVGL  

The firmware ensures real-time control, safe state transitions, and bidirectional communication with a backend server.

## Technologies

- ESP32  
- PlatformIO  
- LVGL  
- LovyanGFX  
- ArduinoJson  
- WebSocketsClient  

## Architecture

The system is structured around:

- **System State Machine** (network & connectivity management)
- **Pump State Machine** (transaction logic)
- **UI Manager** (LVGL interface)
- **WebSocket Manager** (backend communication)

---

Part of a larger smart gas station system including a remote backend server.