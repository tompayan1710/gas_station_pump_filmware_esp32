#pragma once

#include <Arduino.h>

#if __has_include("ConfigLocal.h")
#include "ConfigLocal.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "your_wifi_ssid"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "your_wifi_password"
#endif

#ifndef WS_HOST
#define WS_HOST "127.0.0.1"
#endif

#ifndef WS_PORT
#define WS_PORT 8080
#endif

#ifndef WS_PATH
#define WS_PATH "/"
#endif

// === MOTOR PINS ===
const int RPWM = 4;
const int LPWM = 2;
const int PWM_CHANNEL_R = 0;
const int PWM_CHANNEL_L = 1;
const int PWM_FREQ = 20000;
const int PWM_RESOLUTION = 8;

// === DIGITAL INPUTS ===
const int DIS = 22;

// === ANALOG INPUTS ===
const int POT = 35;

// === KEYPAD PINS ===
const byte KEYPAD_ROW_PINS[4] = {14, 32, 33, 26};
const byte KEYPAD_COL_PINS[3] = {27, 13, 25};

// === NETWORK CONFIGURATION ===
const char CLIENT_TYPE[] = "EQUIPMENT";
const char EQUIPMENT_TYPE[] = "FUEL_PUMP";

// === TIMING CONFIGURATION ===
const unsigned long STARTUP_DELAY_MS = 1000;
const unsigned long MAIN_LOOP_DELAY_MS = 1;

const unsigned long WIFI_CHECK_INTERVAL_MS = 500;
const int WIFI_RETRY_CHECK_COUNT = 10;

const unsigned long WS_RECONNECT_INTERVAL_MS = 5000;
const unsigned long REGISTRATION_RETRY_INTERVAL_MS = 4000;
const unsigned long REGISTRATION_TIMEOUT_MS = 13000;

const unsigned long FUEL_PRICE_TIMEOUT_MS = 5000;
const unsigned long INSERT_CARD_DELAY_MS = 4000;
const unsigned long PIN_VERIFY_DELAY_MS = 1000;
const unsigned long PIN_RESULT_DELAY_MS = 3000;
const unsigned long FUELING_UPDATE_INTERVAL_MS = 100;
const unsigned long FUELING_STOP_TIMEOUT_MS = 4000;
const unsigned long READY_TO_FUEL_DELAY_MS = 3000;
const unsigned long TRANSACTION_RESULT_DELAY_MS = 1000;
const unsigned long RESULT_TO_NEXT_STATE_DELAY_MS = 2000;
const unsigned long FINISHED_SCREEN_DELAY_MS = 4000;
