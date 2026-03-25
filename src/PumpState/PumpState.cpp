#include "PumpState.h"
#include <Arduino.h>

#include "../Config.h"
#include "../SystemState/WebSocketManager.h"
#include "./KeypadManager.h"

#include "../Ui/Ui_Manager.h"
#include "../Ui/Screen_Home.h"
#include "../Ui/Screen_Fuel_Selection.h"
#include "../Ui/Screen_Payment_Type.h"
#include "../Ui/Screen_Payment_Amount.h"
#include "../Ui/Screen_PIN.h"
#include "../Ui/Screen_Verify_Pin.h"
#include "../Ui/Screen_Transaction_Waiting_Auth.h"
#include "../Ui/Screen_Ready_To_Fuel.h"
#include "../Ui/Screen_Pump_Filing.h"
#include "../Ui/Screen_Transaction_Waiting_Complete.h"
#include "../Ui/Screen_Thank_You.h"
#include "../Ui/Screen_HS.h"



// On initialise les variables
PumpState currentPumpState = PUMP_BOOT;
PumpState previousPumpState = PUMP_BOOT;
unsigned long pumpStateTimer = 0;
int pumpDelay = 0;
PumpState pumpDelayNextCurrentPumpState;
String currentTransactionId = "";


void HandlePumpState() {

    static const char *fuelType;
    static float price_per_liter;
    static float amount;
    static const char *paymentType;

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
            
            /*if(millis() - pumpStateTimer > 3000){
                pumpStateTimer = 0;
                currentPumpState = PUMP_SELECT_FUEL;
            }*/

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

                fuelType = "SP95";
                price_per_liter = 1.24;
                pumpStateTimer = 0;

                pumpStateTimer = millis();
                previousPumpState = PUMP_SELECT_FUEL;
            }

            break;
        }
        case PUMP_SELECT_PAYMENT:{
            if(previousPumpState != PUMP_SELECT_PAYMENT && currentPumpState == PUMP_SELECT_PAYMENT){
                load_payment_type_screen();
                Serial.println("Affichage choix carte");

                paymentType = "CC";
                pumpStateTimer = 0;

                pumpStateTimer = millis();
                previousPumpState = PUMP_SELECT_PAYMENT;
            }
            
            break;
        }
        case PUMP_SELECT_AMOUNT:{
            if(previousPumpState != PUMP_SELECT_AMOUNT && currentPumpState == PUMP_SELECT_AMOUNT){
                load_amount_selection_screen();
                Serial.println("Affichage choix amount");

                amount = 100.0;
                pumpStateTimer = 0;

                pumpStateTimer = millis();
                previousPumpState = PUMP_SELECT_AMOUNT;
            }
            
            break;
        }
        case PUMP_WAITING_PIN:{
            static char pin_enter[5] = "";
            static int number_pin_enter = 0;

            if(previousPumpState != PUMP_WAITING_PIN){
                load_pin_screen();
                //sendStartTransactionAuthPacket(fuelType, paymentType, amount);
                Serial.println("Action: Affichage PIN");

                number_pin_enter = 0;
                pin_enter[0] = '\0';

                lv_label_set_text(pin_label, "_   _   _   _");

                pumpStateTimer = millis();
                previousPumpState = PUMP_WAITING_PIN;
            }

            char key = keypad.getKey();

            if(key){

                if(key == '*'){  
                    number_pin_enter = 0;
                    pin_enter[0] = '\0';
                    lv_label_set_text(pin_label, "_   _   _   _");
                    return;
                }

                if(key >= '0' && key <= '9' && number_pin_enter < 4){

                    pin_enter[number_pin_enter] = key;
                    number_pin_enter++;
                    pin_enter[number_pin_enter] = '\0';

                    char buffer[32];

                    if(number_pin_enter == 1) snprintf(buffer,sizeof(buffer),"%c   _   _   _",pin_enter[0]);
                    if(number_pin_enter == 2) snprintf(buffer,sizeof(buffer),"%c   %c   _   _",pin_enter[0],pin_enter[1]);
                    if(number_pin_enter == 3) snprintf(buffer,sizeof(buffer),"%c   %c   %c   _",pin_enter[0],pin_enter[1],pin_enter[2]);
                    if(number_pin_enter == 4) snprintf(buffer,sizeof(buffer),"%c   %c   %c   %c",pin_enter[0],pin_enter[1],pin_enter[2],pin_enter[3]);

                    lv_label_set_text(pin_label, buffer);

                    if(number_pin_enter == 4){
                        Serial.print("PIN entered: ");
                        Serial.println(pin_enter);

                        currentPumpState = PUMP_VERIFY_PIN;
                    }
                }
            }

            break;
        }
       case PUMP_VERIFY_PIN:{
            static unsigned long verifyTimer = 0;
            static bool verifyStarted = false;
            static int pinAttempts = 0;

            if(previousPumpState != PUMP_VERIFY_PIN){
                load_verify_pin();
                Serial.println("Verification du PIN...");

                verifyTimer = millis();
                verifyStarted = true;

                previousPumpState = PUMP_VERIFY_PIN;
            }

            if(verifyStarted && millis() - verifyTimer > 2000){

                bool isPinValid = true; // simulation

                if(isPinValid){
                    Serial.println("PIN correct");
                    pinAttempts = 0;
                    currentPumpState = PUMP_WAITING_AUTH;
                }
                else{
                    pinAttempts++;
                    Serial.println("PIN incorrect");

                    if(pinAttempts >= 3){
                        Serial.println("3 erreurs PIN -> retour accueil");
                        pinAttempts = 0;
                        currentPumpState = PUMP_IDLE;
                    }
                    else{
                        Serial.println("Reessayer PIN");
                        currentPumpState = PUMP_WAITING_PIN;
                    }
                }

                verifyStarted = false;
            }

            break;
        }
        case PUMP_WAITING_AUTH:{
            /*if(previousPumpState != PUMP_WAITING_AUTH){
                load_waiting_auth_screen();
                pumpStateTimer = millis();
                previousPumpState = PUMP_WAITING_AUTH;
            }*/
            currentPumpState = PUMP_READY_TO_FUEL;
            break;
        }

        case PUMP_READY_TO_FUEL:{
            Serial.println("Action: PUMP_READY_TO_FUEL");
            if(previousPumpState != PUMP_READY_TO_FUEL){
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
                ledcWrite(PWM_CHANNEL_R, 150);
                ledcWrite(PWM_CHANNEL_L, 0);

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
            } else {
                ledcWrite(PWM_CHANNEL_R, 0);
                ledcWrite(PWM_CHANNEL_L, 0);
            }

            // Si plus de débit depuis 4 secondes
            if(millis() - lastFlowTime > 4000){
                currentPumpState = PUMP_TRANSACTION_COMPLETE;
            }

            break;
        }
        case PUMP_TRANSACTION_COMPLETE: {
            static unsigned long retryTimer = 0;
            static int retryCount = 0;
            const int maxRetry = 4; // par exemple

            if(previousPumpState != PUMP_TRANSACTION_COMPLETE) {

                float totalPrice = price_per_liter * liters;
                load_transaction_waiting_complete_screen(liters, price_per_liter, totalPrice);

                sendTransactionCompletePacket(currentTransactionId.c_str(), liters);

                retryTimer = millis();
                retryCount = 0;
                previousPumpState = PUMP_TRANSACTION_COMPLETE;
            }

            if(millis() - retryTimer > 4000) {

                if(retryCount < maxRetry) {

                    Serial.println("Retry COMPLETE...");
                    sendTransactionCompletePacket(currentTransactionId.c_str(), liters);

                    retryTimer = millis();
                    retryCount++;

                } else {
                    Serial.println("Max retry atteint !");
                    currentPumpState = PUMP_IDLE;
                    // afficher un écran erreur
                    // idéalement sauvegarder la transaction en flash
                }
            }

            break;
        }
        case PUMP_FINISHED: {
            if(previousPumpState != PUMP_FINISHED) {

                float totalPrice = price_per_liter * liters;
                load_thank_you_screen(liters, totalPrice);

                pumpStateTimer = millis();
                previousPumpState = PUMP_FINISHED;
            }

            if(millis() - pumpStateTimer > 4000){
                currentPumpState = PUMP_IDLE;
            }

            break;
        }
        case PUMP_DELAY: {

            if(previousPumpState != PUMP_DELAY){

                // Ici on vient juste attentre

                pumpStateTimer = millis();
                previousPumpState = PUMP_DELAY;
            }

            // Attendre pumpDelay avant de passer à READY
            if(millis() - pumpStateTimer > pumpDelay){
                currentPumpState = pumpDelayNextCurrentPumpState;
            }

            break;
        } case PUMP_HS: {
            load_hs_screen();
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