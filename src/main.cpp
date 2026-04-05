#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <WebSocketsClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "./Config.h"
#include "./PumpState/PumpState.h"
#include "./SystemState/SystemState.h"
#include "./SystemState/WebSocketManager.h"
#include "./Ui/Ui_Manager.h"

void setup() {
    currentSystemState = SYS_BOOT;
    Serial.begin(115200);
    delay(1000);

    /*
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    delay(50);
    digitalWrite(2, HIGH);
    delay(50);
    */


    tft.init();
    // tft.setRotation(1);
    tft.setRotation(3);

    tft.setSwapBytes(true);

    lv_init();
    lv_display_t * disp = lv_display_create(480, 320);
    lv_display_set_default(disp);
    //lv_tick_set_cb(millis);

    // 3️⃣ Associer le flush
    lv_display_set_flush_cb(disp, my_disp_flush);

    // 4️⃣ Associer le buffer
    lv_display_set_buffers(
        disp,
        buf,
        NULL,
        sizeof(lv_color_t) * LVGL_BUFFER_SIZE,
        LV_DISPLAY_RENDER_MODE_PARTIAL
        // LV_DISPLAY_RENDER_MODE_FULL
    );

    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touch_read);


    ui_init();  

    lv_timer_handler();

    // === DIAGNOSTIC TEMPORAIRE - a supprimer apres test ===
    /*WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("=== Scan reseaux WiFi ===");
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("Aucun reseau trouve - WiFi ESP32 possiblement HS");
    } else {
        for (int i = 0; i < n; i++) {
            Serial.printf("  [%d] SSID: %s  RSSI: %d dBm\n", i+1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
        }
    }
    Serial.println("=== Fin scan ===");*/
    // ======================================================
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
    currentSystemState = SYS_WIFI_CONNECTING;

    ledcSetup(PWM_CHANNEL_R, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(RPWM, PWM_CHANNEL_R);

    ledcSetup(PWM_CHANNEL_L, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(LPWM, PWM_CHANNEL_L);

    ledcWrite(PWM_CHANNEL_R, 0);
    ledcWrite(PWM_CHANNEL_L, 0);

    pinMode(DIS, INPUT);
}

void loop() {
    webSocket.loop();

    
    handleSystemState();
    
    HandlePumpState();

    lv_tick_inc(5);      // <-- AJOUT CRITIQUE
    lv_timer_handler();
    delay(1);

    //HandlePumpLoopingState();
    uint16_t x, y;

    if(tft.getTouch(&x,&y))
    {
        Serial.printf("Touch %d %d\n",x,y);
    }
    /*static unsigned long t = 0;
    static bool toggle = false;

    if (millis() - t > 500) {
        t = millis();
        toggle = !toggle;

        if (toggle) {
            lv_label_set_text(system_label, "REFRESH ON");
            lv_obj_set_style_text_color(system_label, lv_color_hex(0x00FF00), 0);
        } else {
            lv_label_set_text(system_label, "REFRESH OFF");
            lv_obj_set_style_text_color(system_label, lv_color_hex(0xFF0000), 0);
        }
    }*/
}