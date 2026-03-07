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


    tft.init();
    tft.setSwapBytes(true);
    pinMode(22, OUTPUT); // Souvent le pin 22 ou 21 sur les cartes rondes
    digitalWrite(22, HIGH);

    lv_init();
    lv_display_t * disp = lv_display_create(240, 240);
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
    );

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

    currentSystemState = SYS_WIFI_CONNECTING;

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(ENA, PWM_CHANNEL);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(DIS, INPUT);


    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}

void loop() {
    webSocket.loop();

    handleSystemState();
    HandlePumpState();

    lv_tick_inc(5);      // <-- AJOUT CRITIQUE
    lv_timer_handler();
    delay(1);

    //HandlePumpLoopingState();
    
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