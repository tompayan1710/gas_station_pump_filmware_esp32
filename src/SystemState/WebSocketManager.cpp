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
    //webSocket.enableHeartbeat(15000, 3000, 2);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {

        /*case WStype_PING:
            Serial.println("[ESP] Ping reçu du serveur");
            webSocket.sendPong();
            break;*/

        case WStype_PONG:
            Serial.println("[ESP] Pong reçu");
            break;
        case WStype_CONNECTED:
        {

            Serial.println("[ESP] Connecté WS");
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
            Serial.print(typeMsg);


            if (strcmp(typeMsg, "PING") == 0) {
                Serial.println("[ESP] PING reçu");
                StaticJsonDocument<256> doc;
                doc["type"] = "PONG";
                String output;
                serializeJson(doc, output);

                webSocket.sendTXT(output);
                Serial.println("[ESP] Envoie PONG");
            }
            if (strcmp(typeMsg, "IDENTIFY_OK") == 0) {
                
                const char* type = doc["type"];
                JsonObject payloadObj = doc["payload"];

                const char* mac = payloadObj["mac"];

                Serial.print("type : ");
                Serial.println(type);
                Serial.print("mac : ");
                Serial.println(mac);

                String displayText = " \n| mac : ";
                displayText += mac;

                lastWebSocketMessage = String((char*)payload);
                //lv_label_set_text(websocket_label, displayText.c_str());

                char buffer[64];
                snprintf(buffer, sizeof(buffer), "MAC: %s", mac);
                
                if(network_message_label) {
                    lv_label_set_text(network_message_label, buffer);
                }

                previousSystemState = SYS_REGISTERING;
                currentSystemState = SYS_READY;
            }else if (strcmp(typeMsg, "ERROR_DB") == 0) {
                const char* message = doc["message"] | "Erreur DB";
                /*if(db_label) {
                    lv_label_set_text(db_label, message);
                    lv_obj_set_style_text_color(db_label, lv_color_hex(0xFF0000), 0);
                }*/
                previousSystemState = SYS_REGISTERING;
                currentSystemState = SYS_REGISTERING;
            }else if(strcmp(typeMsg, "CARBURANT_PRICE") == 0){

                const char* sku = doc["payload"]["sku"];
                float prix = doc["payload"]["prix"];

                Serial.printf("Prix reçu %s : %.2f\n", sku, prix);

                if(strcasecmp(sku, "SP95") == 0){
                    price_SP95 = prix;
                }
                else if(strcasecmp(sku, "DIESEL") == 0){
                    price_DIESEL = prix;
                }
            }
            else if(strcmp(typeMsg, "CARBURANT_CONFIG") == 0){
                
                float min = doc["payload"]["livraisonMinimal"];

                Serial.printf("Montant minimal reçu : %.2f\n", min);

                min_liter = min;
                Serial.printf("min_liter actuel: %.2f\n", min_liter);
            }
            else if(strcmp(typeMsg, "TRANSACTION_AUTH_OK") == 0){
                JsonObject payloadObj = doc["payload"];
                const char *id_transaction = payloadObj["transaction_id"];

                currentTransactionId = String(id_transaction);

                Serial.print("Transaction ID: ");
                Serial.println(currentTransactionId);

                pumpDelay = 3000;
                pumpDelayNextCurrentPumpState = PUMP_READY_TO_FUEL;
                currentPumpState = PUMP_DELAY;
            } else if(strcmp(typeMsg, "TRANSACTION_AUTH_REFUSED") == 0){
                currentTransactionId = "";

                currentPumpState = PUMP_IDLE;
            } else if(strcmp(typeMsg, "TRANSACTION_COMPLETE_OK") == 0){
                currentTransactionId = "";
                previousPumpState = PUMP_TRANSACTION_COMPLETE;
                currentPumpState = PUMP_FINISHED;
                
            } else if(strcmp(typeMsg, "TRANSACTION_COMPLETE_ERROR") == 0){

                Serial.println("Erreur COMPLETE - retry...");

                currentPumpState = PUMP_TRANSACTION_COMPLETE;
            }
            else if(strcmp(typeMsg, "PUMP_TOGGLE") == 0){
                JsonObject payloadObj = doc["payload"];

                const char *status = payloadObj["status"];
                
                Serial.print("PUMP_TOGGLE status : ");
                Serial.println(status);

                if(strcmp(status, "Disponible") == 0){
                    currentPumpState = PUMP_IDLE;
                }
                else if(strcmp(status, "Hors_Service") == 0){
                    should_broadcast_HS = false;
                    currentPumpState = PUMP_HS;
                }
            }



            //Erreur Code
            else if(strcmp(typeMsg, "CARBURANT_PRICE_ERROR") == 0){

                const char* message = doc["payload"]["message"];

                Serial.print("Erreur carburant: ");
                Serial.println(message);

                should_broadcast_HS = true;
                currentPumpState = PUMP_HS;
            }
            break;
        }

        case WStype_DISCONNECTED:
        {
            Serial.println("[ESP] Déconnecté WS");
            currentSystemState = SYS_WIFI_CONNECTING;
            currentPumpState = PUMP_HS;
            break;
        }
    }
}

void sendRegistrationPacket(){
    StaticJsonDocument<128> doc;
    doc["type"] = "IDENTIFY";

    JsonObject payloadObj = doc.createNestedObject("payload");
    payloadObj["client_type"] = "EQUIPMENT";
    payloadObj["mac"] = WiFi.macAddress();
    payloadObj["equipment_type"] = "FUEL_PUMP"; // Ou EV_CHARGER -> Electric Vehicle Charger 

    String output;
    serializeJson(doc, output);

    Serial.println("Envoi IDENTIFY");
    webSocket.sendTXT(output);
}


void requestFuelPrices()
{
    // SP95
    {
        StaticJsonDocument<128> doc;
        doc["type"] = "GET_CARBURANT_PRICE";

        JsonObject payload = doc.createNestedObject("payload");
        payload["sku"] = "SP95";

        String json;
        serializeJson(doc, json);
        webSocket.sendTXT(json);
    }

    // DIESEL
    {
        StaticJsonDocument<128> doc;
        doc["type"] = "GET_CARBURANT_PRICE";

        JsonObject payload = doc.createNestedObject("payload");
        payload["sku"] = "Diesel";

        String json;
        serializeJson(doc, json);
        webSocket.sendTXT(json);
    }
}


void requestCarburantConfig()
{
    StaticJsonDocument<128> doc;
    doc["type"] = "GET_CARBURANT_CONFIG";
    String json;
    serializeJson(doc, json);
    webSocket.sendTXT(json);
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

void sendTransactionCompletePacket(const char* transactionId, float liters){
    StaticJsonDocument<256> doc;

    doc["type"] = "TRANSACTION_COMPLETE";

    JsonObject payloadObj = doc.createNestedObject("payload");
    payloadObj["transaction_id"] = transactionId;
    payloadObj["liters"] = liters;

    String output;
    serializeJson(doc, output);

    webSocket.sendTXT(output);
}

void broadcastPumpStatus(const char *status)
{
    StaticJsonDocument<128> doc;
    doc["type"] = "PUMP_STATUS_EQUIPMENT_UPDATE";

    JsonObject payload = doc.createNestedObject("payload");
    payload["mac"] = WiFi.macAddress();
    payload["status"] = status;

    String json;
    serializeJson(doc, json);
    webSocket.sendTXT(json);
}
