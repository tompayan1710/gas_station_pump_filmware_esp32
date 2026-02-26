#include "WebSocketManager.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "SystemState.h"
#include "../PumpState/PumpState.h"

#include "./Ui/Ui_Manager.h"

WebSocketsClient webSocket;
String lastWebSocketMessage = "";

void initWebSocket() {
    Serial.println("Begin WebSocket");
    webSocket.begin("172.20.10.2", 8080, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {

        case WStype_CONNECTED:
        {
            if(websocket_label) {
                lv_label_set_text(websocket_label, "WS: Connectee !");
                lv_obj_set_style_text_color(websocket_label, lv_color_hex(0x00FF00), 0); // Vert
            }
            previousSystemState = SYS_WS_CONNECTING;
            currentSystemState = SYS_REGISTERING;
            break;
        }

        case WStype_TEXT:
        {
            StaticJsonDocument<256> doc;
            
            if (deserializeJson(doc, payload)) return;
            if (!doc.containsKey("type")) return;
            
            
            const char* typeMsg = doc["type"];
            Serial.print("type : ");
            Serial.print("typeMsg");

            if (strcmp(typeMsg, "PUMP_CONNECTED") == 0) {
                
                const char* type = doc["type"];
                JsonObject payloadObj = doc["payload"];

                const char* mac = payloadObj["mac"];
                const char* pump_type = payloadObj["pump_type"];

                Serial.print("type : ");
                Serial.println(type);
                Serial.print("mac : ");
                Serial.println(mac);
                Serial.print("pump_type : ");
                Serial.println(pump_type);


                String displayText = "pump_type : ";
                displayText += pump_type;
                displayText += " \n| mac : ";
                displayText += mac;

                lastWebSocketMessage = String((char*)payload);
                //lv_label_set_text(websocket_label, displayText.c_str());

                char buffer[64];
                snprintf(buffer, sizeof(buffer), "Pompe: %s\nMAC: %s", pump_type, mac);
                
                if(network_message_label) {
                    lv_label_set_text(network_message_label, buffer);
                }

                previousSystemState = SYS_REGISTERING;
                currentSystemState = SYS_READY;
            }
            if (strcmp(typeMsg, "ERROR_DB") == 0) {
                const char* message = doc["message"] | "Erreur DB";
                /*if(db_label) {
                    lv_label_set_text(db_label, message);
                    lv_obj_set_style_text_color(db_label, lv_color_hex(0xFF0000), 0);
                }*/
                previousSystemState = SYS_REGISTERING;
                currentSystemState = SYS_REGISTERING;
            }


            if(strcmp(typeMsg, "TRANSACTION_AUTH_OK") == 0){
                JsonObject payloadObj = doc["payload"];
                const char *id_transaction = payloadObj["transaction_id"];

                currentTransactionId = String(id_transaction);

                Serial.print("Transaction ID: ");
                Serial.println(currentTransactionId);

                currentPumpState = PUMP_READY_TO_FUEL;
            } 
            break;
        }

        case WStype_DISCONNECTED:
        {
            Serial.println("WebSocket disconnected");
            currentSystemState = SYS_WIFI_CONNECTING;
            break;
        }
    }
}

void sendRegistrationPacket(){
    StaticJsonDocument<128> doc;
    doc["type"] = "CONNECT_PUMP";

    JsonObject payloadObj = doc.createNestedObject("payload");
    payloadObj["mac"] = WiFi.macAddress();
    payloadObj["pump_type"] = "PUMP";

    String output;
    serializeJson(doc, output);

    Serial.println("Envoi CONNECT_PUMP");
    webSocket.sendTXT(output);
}

void sendStartTransactionAuthPacket(const char* fuelType, const char* paymentType, float amount){
    StaticJsonDocument<256> doc;

    doc["type"] = "AUTH_TRANSACTION";

    JsonObject payloadObj = doc.createNestedObject("payload");
    payloadObj["mac"] = WiFi.macAddress();
    payloadObj["fuel_type"] = fuelType;      // SP95 / DIESEL
    payloadObj["payment_type"] = paymentType; // CB / CCE
    payloadObj["amount"] = amount;            // montant choisi

    String output;
    serializeJson(doc, output);

    webSocket.sendTXT(output);
}

void sendTransactionCompletePacket(const char* transactionId, float liters, float totalPrice){
    StaticJsonDocument<256> doc;

    doc["type"] = "TRANSACTION_COMPLETE";

    JsonObject payloadObj = doc.createNestedObject("payload");
    payloadObj["transaction_id"] = transactionId;
    payloadObj["liters"] = liters;
    payloadObj["total_price"] = totalPrice;

    String output;
    serializeJson(doc, output);

    webSocket.sendTXT(output);
}