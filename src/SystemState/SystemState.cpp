#include "SystemState.h"
#include <Arduino.h>
#include <WiFi.h>
#include "./WebSocketManager.h"
#include "../PumpState/PumpState.h"
#include "../Ui/Ui_Manager.h"

SystemState currentSystemState = SYS_BOOT;
SystemState previousSystemState = SYS_BOOT;
unsigned long systemStateTimer = 0;

void handleSystemState(){
    static unsigned long lastCheck = 0;
    static unsigned long nbCheck = 0;

    switch (currentSystemState){
        case SYS_WIFI_CONNECTING:
            if (previousSystemState != SYS_WIFI_CONNECTING) {
                if(wifi_label) lv_label_set_text(wifi_label, "WiFi: Connexion...");
                Serial.println("Connecting WiFi...");
                WiFi.mode(WIFI_STA);
                // Serial.println("WiFi skipped");
                WiFi.begin("iphonetom", "345566778");
                previousSystemState = SYS_WIFI_CONNECTING;
                lastCheck = 0;
                nbCheck = 0;
            }

            if (millis() - lastCheck >= 500) {
                lastCheck = millis();
                nbCheck++;

                if (WiFi.status() == WL_CONNECTED) {
                    if(wifi_label) {
                        lv_label_set_text(wifi_label, "WiFi: Connectee !");
                        lv_obj_set_style_text_color(wifi_label, lv_color_hex(0x00FF00), 0);
                    }
                    Serial.println("WiFi connected");
                    Serial.print("IP address: ");
                    Serial.println(WiFi.localIP());

                    previousSystemState = currentSystemState;
                    currentSystemState = SYS_WS_CONNECTING;
                } else if (nbCheck > 5 && nbCheck % 10 == 0) {
                    // Toutes les 10s supplementaires, on retente
                    Serial.printf("WiFi retry #%lu...\n", nbCheck / 20);
                    WiFi.reconnect();
                }
            }
            break;
        case SYS_WS_CONNECTING:
            if (previousSystemState != SYS_WS_CONNECTING) {
                if(websocket_label) {
                    lv_label_set_text(websocket_label, "WS: Connexion...");
                    lv_obj_set_style_text_color(websocket_label, lv_color_hex(0xFFA500), 0); // Orange
                }
                initWebSocket();
                previousSystemState = SYS_WS_CONNECTING;
            }
            //vTaskDelay(100 / portTICK_PERIOD_MS);
            break;
        case SYS_REGISTERING:
            // On n'entre ici que si le WebSocket est connecté
            if (previousSystemState != SYS_REGISTERING) {
                if(websocket_label) {
                    lv_label_set_text(db_label, "BD: Enregistrement...");
                    lv_obj_set_style_text_color(db_label, lv_color_hex(0xFFA500), 0);
                }
                
                // On crée une fonction pour envoyer le JSON (voir point 3)
                sendRegistrationPacket(); 
                
                previousSystemState = SYS_REGISTERING;
                currentSystemState = SYS_REGISTERING;
                systemStateTimer = millis(); // On lance un chrono
            }else{
                //Si après 13 secondes on n'a pas de réponse du serveur, on reboot
                if (millis() - systemStateTimer > 13000) {
                    Serial.println("Critical timeout - Resetting WS");
                    if(db_label){
                        lv_label_set_text(db_label, "Erreur: Timeout BD");
                        lv_obj_set_style_text_color(db_label, COLOR_WHITE_BG, 0);
                    }
                    if(websocket_label){
                        lv_label_set_text(websocket_label, "WS: En attente");
                         lv_obj_set_style_text_color(websocket_label, COLOR_WHITE_BG, 0);
                    }
                    previousSystemState = SYS_BOOT; 
                    currentSystemState = SYS_WS_CONNECTING;
                    //vTaskDelay(100 / portTICK_PERIOD_MS);
                    break;
                }

                //On entre ici car previousSystemState == SYS_REGISTERING
                if(millis() - lastCheck > 4000){
                    nbCheck++;
                    char buffer[64];
                    snprintf(buffer, sizeof(buffer), "BD: Tentative %lu...", nbCheck);
                    if(db_label) lv_label_set_text(db_label, buffer);
                    
                    Serial.print("Registration timeout ");
                    Serial.print(nbCheck);
                    Serial.println(" - BD: Enregistrement...");
                    sendRegistrationPacket();
                    lastCheck = millis(); // On reset UNIQUEMENT le chrono de l'intervalle
                }
            }

            break;
        case SYS_READY:
            if (previousSystemState != SYS_READY) {
                if(db_label) {
                    lv_label_set_text(db_label, "BD: Connectee !");
                    lv_obj_set_style_text_color(db_label, lv_color_hex(0x00FF00), 0);
                }

                if(system_label) {
                    lv_label_set_text(system_label, "Systeme : Operationnel !");
                    lv_obj_set_style_text_color(system_label, lv_color_hex(0x00FF00), 0);
                }

                previousSystemState = SYS_READY;

                currentPumpState = PUMP_IDLE;
            }
            //vTaskDelay(100 / portTICK_PERIOD_MS);
            break;
        default:
            break;
    }
}
