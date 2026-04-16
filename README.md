# Connected Fuel Pump Firmware

This project is an ESP32 firmware for a connected fuel pump simulator. The board displays a local touch interface, manages the pump workflow, and exchanges JSON messages with a backend server through WebSocket.

The goal is to demonstrate a clean embedded architecture with three main responsibilities:

- user interface on the screen
- local pump logic with a state machine
- network communication with a local backend server

The project is designed for a local demo setup: the ESP32 and the server must be connected to the same Wi-Fi network so they can communicate with local IP addresses.

## What The Firmware Does

The firmware manages a full transaction flow:

- idle screen
- fuel selection
- payment selection
- card and PIN flow
- authorization request
- fueling simulation
- transaction completion

During the process, the ESP32 sends requests to the server and waits for responses over WebSocket.

## Main Technologies

- ESP32
- C++
- PlatformIO
- LVGL
- LovyanGFX
- ArduinoJson
- WebSocketsClient
- Keypad

## Global Architecture

The current architecture is good for this project and does not need a big redesign.

### 1. System State Machine

This part manages:

- Wi-Fi connection
- WebSocket connection
- device registration on the backend

Main files:

- `src/SystemState/SystemState.cpp`
- `src/SystemState/WebSocketManager.cpp`

### 2. Pump State Machine

This part manages:

- the customer journey
- transaction data
- local transitions between screens and steps

Main file:

- `src/PumpState/PumpState.cpp`

### 3. UI Layer

This part manages:

- display initialization
- touch handling
- screen loading

Main files:

- `src/Ui/Ui_Manager.cpp`
- `src/Ui/Screen_*.cpp`

## Project Structure

```text
src/
|-- main.cpp
|-- Config.h
|-- ConfigLocal.example.h
|-- Assets/
|-- PumpState/
|   |-- PumpState.cpp
|   |-- PumpState.h
|   |-- KeypadManager.cpp
|   `-- KeypadManager.h
|-- SystemState/
|   |-- SystemState.cpp
|   |-- SystemState.h
|   |-- WebSocketManager.cpp
|   `-- WebSocketManager.h
`-- Ui/
    |-- Ui_Manager.cpp
    |-- Ui_Manager.h
    `-- Screen_*.cpp / .h
```

## Important Files

### `src/Config.h`

Contains:

- pin mapping
- timing constants
- fallback network values

### `src/ConfigLocal.example.h`

Template for local configuration. This file must be copied to `src/ConfigLocal.h` and filled with your own Wi-Fi and server values.

### `src/main.cpp`

Application entry point:

- initializes hardware
- initializes the UI
- starts the main loop

### `src/SystemState/SystemState.cpp`

Handles:

- Wi-Fi connection state
- WebSocket startup
- registration flow

### `src/SystemState/WebSocketManager.cpp`

Handles:

- JSON packet sending
- JSON packet reception
- backend response handling

### `src/PumpState/PumpState.cpp`

Handles:

- pump workflow
- transaction values
- local state transitions

## Local Network Requirement

This point is very important for the demo.

The backend server and the ESP32 must be connected to the same local Wi-Fi network.

Why:

- the ESP32 connects to the server with the local IP address defined in `WS_HOST`
- if the server is on another network, the ESP32 will not be able to reach it in local mode

Example:

- computer running the server: `192.168.1.10`
- ESP32 connected to the same Wi-Fi network
- `WS_HOST` in `ConfigLocal.h`: `"192.168.1.10"`

In short:

- same Wi-Fi
- same local network
- known local IP for the server

## Local Configuration

The network settings are intentionally not stored in the shared source code.  
Use a local file instead.

### Step 1: Create `ConfigLocal.h`

Copy:

- `src/ConfigLocal.example.h`

to:

- `src/ConfigLocal.h`

### Step 2: Fill Your Local Values

Example:

```cpp
#pragma once

#define WIFI_SSID "your_wifi_name"
#define WIFI_PASSWORD "your_wifi_password"

#define WS_HOST "192.168.1.10"
#define WS_PORT 8080
#define WS_PATH "/"
```

Notes:

- `WS_HOST` must be the local IP address of the computer running the server
- `src/ConfigLocal.h` is ignored by Git

## How To Build And Flash

### Requirements

- PlatformIO installed
- ESP32 board connected by USB
- backend server available on the same Wi-Fi network

### 1. Build The Firmware

```bash
pio run
```

### 2. Flash The ESP32

```bash
pio run --target upload
```

If PlatformIO does not detect the serial port automatically, set the correct port in `platformio.ini`.

### 3. Open The Serial Monitor

```bash
pio device monitor
```

The serial monitor is useful to check:

- Wi-Fi connection
- WebSocket connection
- backend responses
- state changes

## Typical Demo Workflow

1. Start the backend server on your computer.
2. Check the local IP address of the computer.
3. Put that IP address in `WS_HOST` inside `src/ConfigLocal.h`.
4. Connect the ESP32 to the same Wi-Fi network.
5. Build the project with `pio run`.
6. Flash the board with `pio run --target upload`.
7. Open the serial monitor with `pio device monitor`.
8. Test the pump flow from the touch screen.

## Hardware Notes

Important pin detail:

- `LPWM` uses GPIO `2`
- keypad pins keep their dedicated mapping

This removes the old conflict with GPIO `33`.

## Current Limits

The code is clean and presentable for a recruiter, but a few simple improvement points still exist:

- PIN validation is still simulated locally
- transaction completion could be more robust if the server does not answer
- there are no automated tests yet

These points do not require an architecture rewrite. They are improvements, not structural problems.

## Clean Code Choices Applied

The cleanup was intentionally kept simple:

- configuration grouped in one place
- local secrets moved out of shared source files
- timing values replaced by named constants
- pin conflict fixed
- state reset improved when returning to idle
- README rewritten for easier onboarding

## Conclusion

This project is now organized in a clear and professional way for a demo or recruiter review:

- the architecture is understandable
- the code stays beginner-friendly
- the setup is reproducible
- the local network requirement is explicit
- the flashing steps are documented
