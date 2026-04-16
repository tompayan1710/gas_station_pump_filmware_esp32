#include <Arduino.h>
#include <ArduinoJson.h>
#include <LovyanGFX.hpp>
#include <WebSocketsClient.h>
#include <WiFi.h>
#include <lvgl.h>

#include "./Config.h"
#include "./PumpState/PumpState.h"
#include "./SystemState/SystemState.h"
#include "./SystemState/WebSocketManager.h"
#include "./Ui/Ui_Manager.h"

uint32_t my_tick_get_cb(void) {
    return millis();
}

void initDisplay() {
    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(false);

    lv_init();

    lv_display_t *disp = lv_display_create(480, 320);
    lv_display_set_default(disp);
    lv_tick_set_cb(my_tick_get_cb);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(
        disp,
        buf,
        NULL,
        sizeof(lv_color_t) * LVGL_BUFFER_SIZE,
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touch_read);

    ui_init();
    lv_timer_handler();
}

void initMotorPins() {
    ledcSetup(PWM_CHANNEL_R, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(RPWM, PWM_CHANNEL_R);

    ledcSetup(PWM_CHANNEL_L, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(LPWM, PWM_CHANNEL_L);

    ledcWrite(PWM_CHANNEL_R, 0);
    ledcWrite(PWM_CHANNEL_L, 0);

    pinMode(DIS, INPUT);
}

void setup() {
    currentSystemState = SYS_BOOT;
    Serial.begin(115200);
    delay(STARTUP_DELAY_MS);

    initDisplay();

    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    currentSystemState = SYS_WIFI_CONNECTING;

    initMotorPins();
}

void loop() {
    webSocket.loop();
    handleSystemState();
    HandlePumpState();

    lv_timer_handler();
    delay(MAIN_LOOP_DELAY_MS);
}
