#include "PumpState.h"
#include <Arduino.h>

#include "../Config.h"
#include "../SystemState/WebSocketManager.h"

#include "../Ui/Ui_Manager.h"
#include "../Ui/Screen_Home.h"
#include "../Ui/Screen_Fuel_Selection.h"
#include "../Ui/Screen_Payment_Type.h"
#include "../Ui/Screen_Payment_Amount.h"

#include "../Ui/Screen_Waiting_Auth.h"

#include "../Ui/Screen_Ready_To_Fuel.h"
#include "../Ui/Screen_Pump_Filing.h"
#include "../Ui/Screen_Finish.h"

// On initialise les variables
PumpState currentPumpState = PUMP_BOOT;
PumpState previousPumpState = PUMP_BOOT;
unsigned long pumpStateTimer = 0;
String currentTransactionId = "";

void HandlePumpState() {

    static char *fuelType;
    static float fuelPrice;
    static float amount;
    static char *paymentType;

    static float liters = 0.0;
    static unsigned long lastFlowTime = 0;
    static unsigned long lastUpdateTime = 0;

    switch (currentPumpState) {
        case PUMP_IDLE:{
            
            if(previousPumpState != PUMP_IDLE && currentPumpState == PUMP_IDLE){
                load_home_screen(); 
                pumpStateTimer = millis();
                previousPumpState = PUMP_IDLE;
            } 
            
            if(millis() - pumpStateTimer > 3000){
                pumpStateTimer = 0;
                currentPumpState = PUMP_SELECT_FUEL;
            }

            /*
            int valPot = map(analogRead(POT), 0, 4095, 0, 255);
            ledcWrite(PWM_CHANNEL, valPot);
            Serial.println("Moteur on");
            Serial.println(valPot);*/
            //Serial.println("Affichage Accueil");
            break;
        }
        case PUMP_SELECT_FUEL:{
            if(previousPumpState != PUMP_SELECT_FUEL && currentPumpState == PUMP_SELECT_FUEL){
                load_fuel_selection_screen();
                Serial.println("Affichage choix carburant");

                pumpStateTimer = millis();
                previousPumpState = PUMP_SELECT_FUEL;
            }

            if(millis() - pumpStateTimer > 3000){
                fuelType = "SP95";
                fuelPrice = 1.24;
                pumpStateTimer = 0;
                currentPumpState = PUMP_SELECT_PAYMENT;
            }

            break;
        }
        case PUMP_SELECT_PAYMENT:{
            if(previousPumpState != PUMP_SELECT_PAYMENT && currentPumpState == PUMP_SELECT_PAYMENT){
                load_payment_type_screen();
                Serial.println("Affichage choix carte");

                pumpStateTimer = millis();
                previousPumpState = PUMP_SELECT_PAYMENT;
            }
            
            if(millis() - pumpStateTimer > 3000){
                paymentType = "CC";
                pumpStateTimer = 0;
                currentPumpState = PUMP_SELECT_AMOUNT;
            }
            break;
        }
        case PUMP_SELECT_AMOUNT:{
            if(previousPumpState != PUMP_SELECT_AMOUNT && currentPumpState == PUMP_SELECT_AMOUNT){
                load_amount_selection_screen();
                Serial.println("Affichage choix amount");

                pumpStateTimer = millis();
                previousPumpState = PUMP_SELECT_AMOUNT;
            }
            
            if(millis() - pumpStateTimer > 3000){
                amount = 100.0;
                pumpStateTimer = 0;
                currentPumpState = PUMP_WAITING_PAYMENT;
            }
            break;
        }
        case PUMP_WAITING_PAYMENT:{
            // load_payment_screen();
            currentPumpState = PUMP_WAITING_AUTH;
            
            Serial.println("Action: Affichage 'Insérez carte'");
            sendStartTransactionAuthPacket(fuelType, paymentType, amount);
            previousPumpState = PUMP_WAITING_PAYMENT;
            currentPumpState = PUMP_WAITING_AUTH;
            break;
        }
        case PUMP_WAITING_AUTH:{
            if(previousPumpState != PUMP_WAITING_AUTH){
                load_waiting_auth_screen();
                pumpStateTimer = millis();
                previousPumpState = PUMP_WAITING_AUTH;
            }
            break;
        }

        case PUMP_READY_TO_FUEL:{
            Serial.println("Action: PUMP_READY_TO_FUEL");
            if(previousPumpState != PUMP_READY_TO_FUEL && currentPumpState == PUMP_READY_TO_FUEL){
                load_action_prompt();
                previousPumpState = PUMP_READY_TO_FUEL;
            }

            if (digitalRead(DIS) == HIGH) {
                Serial.println("Appuye !");
                currentPumpState = PUMP_FUELING;
            }
            break;
        }
        case PUMP_FUELING:{
            if(previousPumpState != PUMP_FUELING){
                load_progress_screen();
                liters = 0.0;
                lastFlowTime = millis();
                lastUpdateTime = millis();
                previousPumpState = PUMP_FUELING;
            }
            Serial.println(digitalRead(DIS));

            // Si signal HIGH = débit
            if(digitalRead(DIS) == HIGH){

                lastFlowTime = millis();

                if(millis() - lastUpdateTime > 100){ // update toutes les 100ms
                    lastUpdateTime = millis();
                    liters += 0.02; // simulation débit

                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "%.2f L", liters);
                    lv_label_set_text(progress_label, buffer);

                    int percent = (int)(liters * 2); // juste visuel
                    if(percent > 100) percent = 100;
                    lv_bar_set_value(progress_bar, percent, LV_ANIM_OFF);
                }
            }

            // Si plus de débit depuis 5 secondes
            if(millis() - lastFlowTime > 5000){
                currentPumpState = PUMP_FINISHED;
            }

            break;
        }
        case PUMP_FINISHED:{

            if(previousPumpState != PUMP_FINISHED){
                load_finished_screen(liters);
                previousPumpState = PUMP_FINISHED;
                pumpStateTimer = millis();

                sendTransactionCompletePacket(currentTransactionId.c_str(), liters, fuelPrice * liters);
            }

            // Retour automatique après 4 secondes
            if(millis() - pumpStateTimer > 4000){
                currentPumpState = PUMP_IDLE;
            }

            break;
        }
        default:{
            break;
        }
    }
}

void HandlePumpLoopingState() {
    // Ici on gère ce qui doit être vérifié en permanence
    switch(currentPumpState) {
        case PUMP_FUELING:
            // TODO: Lire le capteur de débit ici
            break;

        case PUMP_FINISHED:
            // Retour à l'accueil auto après 8 secondes
            if (millis() - pumpStateTimer > 8000) {
                currentPumpState = PUMP_IDLE;
            }
            break;

        default:
            break;
    }
}