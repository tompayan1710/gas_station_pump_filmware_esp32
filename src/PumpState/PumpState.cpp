#include "PumpState.h"
#include <Arduino.h>

#include "../Config.h"
#include "../SystemState/WebSocketManager.h"
#include "./KeypadManager.h"

#include "../Ui/Ui_Manager.h"
#include "../Ui/Screen_Home.h"
#include "../Ui/Screen_Fuel_Selection.h"
#include "../Ui/Screen_Payment_Type.h"
#include "../Ui/Screen_Insert_Card.h"
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


float price_SP95 = -1.0;
float price_DIESEL = -1.0;

const char *fuelType = "";
float price_per_liter = 0.0;
float min_liter = -1.0;
float min_amount = -1.0;
float amount = -1.0;

const char *paymentType = "";

bool is_transaction_complete = false;
bool should_broadcast_HS = false;

void HandlePumpState() {
    static float liters = 0.0;
    static unsigned long lastFlowTime = 0;
    static unsigned long lastUpdateTime = 0;

    static bool fuelScreenLoaded = false;

    switch (currentPumpState) {
        case PUMP_IDLE:{
            
            if(previousPumpState != PUMP_IDLE && currentPumpState == PUMP_IDLE){
                min_liter = -1;
                min_amount = -1;
                pumpStateTimer = 0;

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
            if(previousPumpState != PUMP_SELECT_FUEL){

                requestFuelPrices();
                pumpStateTimer = millis();

                previousPumpState = PUMP_SELECT_FUEL;
            }

            // OK → afficher UNE SEULE FOIS puis sortir du state
            if(price_SP95 > 0 && price_DIESEL > 0 && !fuelScreenLoaded){

                
                Serial.println("Prix OK → affichage");
                
                load_fuel_selection_screen();
                fuelScreenLoaded = true;

                previousPumpState = PUMP_SELECT_FUEL;

                // 🔥 ON RESTE dans cet état sans recharger
                break;
            }

            // TIMEOUT uniquement si PAS reçu
            if(price_SP95 <= 0 || price_DIESEL <= 0){
                if(millis() - pumpStateTimer > 5000){
                    Serial.println("Timeout prix carburant");
                    should_broadcast_HS = true;
                    currentPumpState = PUMP_HS;
                }
            }

            break;
        }
        case PUMP_SELECT_PAYMENT:{
            if(previousPumpState != PUMP_SELECT_PAYMENT && currentPumpState == PUMP_SELECT_PAYMENT){
                load_payment_type_screen();
                Serial.println("Affichage choix carte");
                previousPumpState = PUMP_SELECT_PAYMENT;
            }
            
            break;
        }
        case PUMP_INSERT_CARD:{
            if(previousPumpState != PUMP_INSERT_CARD && currentPumpState == PUMP_INSERT_CARD){
                load_insert_card_screen();
                Serial.println("Affichage PUMP_INSERT_CARD");

                pumpStateTimer = millis();
                previousPumpState = PUMP_INSERT_CARD;
            } if(millis() - pumpStateTimer > 4000){
                currentPumpState = PUMP_WAITING_PIN;
            }
            
            break;
        }
        case PUMP_WAITING_PIN:{
            static char pin_enter[5] = "";
            static int number_pin_enter = 0;

            if(previousPumpState != PUMP_WAITING_PIN){
                load_pin_screen();
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
            static bool resultShown = false;
            static bool isPinValid = false;
            static int pinAttempts = 0;

            if(previousPumpState != PUMP_VERIFY_PIN){
                load_verify_pin();

                verifyTimer = millis();
                verifyStarted = true;
                resultShown = false;

                previousPumpState = PUMP_VERIFY_PIN;
            }

            // Phase 1 → spinner (1 seconde)
            if(verifyStarted && !resultShown && millis() - verifyTimer > 1000){

                isPinValid = true; // simulation

                hide_verify_spinner(); // 🔥 IMPORTANT

                if(isPinValid){
                    update_verify_pin_text("CODE BON", true);
                } else {
                    update_verify_pin_text("CODE INVALIDE", false);
                }

                resultShown = true;
                verifyTimer = millis();
            }

            // Phase 2 → afficher résultat (2 secondes)
            if(resultShown && millis() - verifyTimer > 3000){

                if(isPinValid){
                    pinAttempts = 0;
                    currentPumpState = PUMP_WAITING_AUTH;
                }
                else{
                    pinAttempts++;

                    if(pinAttempts >= 3){
                        pinAttempts = 0;
                        currentPumpState = PUMP_IDLE;
                    } else {
                        currentPumpState = PUMP_WAITING_PIN;
                    }
                }

                verifyStarted = false;
            }

            break;
        }
        case PUMP_WAITING_AUTH:{
            
            if(previousPumpState != PUMP_WAITING_AUTH){
                Serial.println("PUMP_WAITING_AUTH");
                requestCarburantConfig();
                pumpStateTimer = 0;
                previousPumpState = PUMP_WAITING_AUTH;
            }

            if(min_liter > 0 && pumpStateTimer == 0){

                min_amount = min_liter * price_per_liter;
                Serial.println("sendStartTransactionAuthPacket");
                sendStartTransactionAuthPacket(fuelType, paymentType, min_amount);
                load_waiting_auth_screen();

                pumpStateTimer = millis(); // bloque la répétition
            }

            break;
        }

        case PUMP_READY_TO_FUEL:{
            if(previousPumpState != PUMP_READY_TO_FUEL){
                Serial.println("Action: PUMP_READY_TO_FUEL");
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

                if(millis() - lastUpdateTime > 100){
                    lastUpdateTime = millis();
                    liters += 0.05;

                    float price_ht = liters * price_per_liter;
                    float price_ttc = price_ht * 1.20;

                    char buffer[32];

                    // Litres
                    snprintf(buffer, sizeof(buffer), "%.2f L", liters);
                    lv_label_set_text(progress_label, buffer);

                    // HT
                    snprintf(buffer, sizeof(buffer), "HT: %.2f EUR", price_ht);
                    lv_label_set_text(price_ht_label, buffer);

                    // TTC
                    snprintf(buffer, sizeof(buffer), "TTC: %.2f EUR", price_ttc);
                    lv_label_set_text(price_ttc_label, buffer);

                    int percent = (int)(price_ht * 2);
                    if(percent > 100) percent = 100;

                    lv_bar_set_value(progress_bar, percent, LV_ANIM_OFF);

                    // Correction logique minimum
                    if(price_ht >= min_amount){
                        lv_label_set_text(min_label, "Minimum atteint");
                        lv_obj_set_style_text_color(min_label, COLOR_GREEN, 0);
                        lv_obj_set_style_text_color(progress_label, COLOR_GREEN, 0);
                        lv_obj_set_style_bg_color(progress_bar, COLOR_GREEN, LV_PART_INDICATOR);
                    }
                    else{
                        lv_obj_set_style_bg_color(progress_bar, COLOR_RED, LV_PART_INDICATOR);
                    }
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
        /*case PUMP_TRANSACTION_COMPLETE: {
            static unsigned long retryTimer = 0;
            static int retryCount = 0;
            const int maxRetry = 4;

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
        }*/
        case PUMP_TRANSACTION_COMPLETE: {
            static unsigned long retryTimer = 0;
            static int retryCount = 0;
            const int maxRetry = 4;

            if(previousPumpState != PUMP_TRANSACTION_COMPLETE) {

                float totalPrice = price_per_liter * liters;

                load_transaction_waiting_complete_screen(liters, price_per_liter, totalPrice);

                sendTransactionCompletePacket(currentTransactionId.c_str(), liters);

                retryTimer = millis();
                retryCount = 0;
                previousPumpState = PUMP_TRANSACTION_COMPLETE;
            }
            break;
        }
        case PUMP_TRANSACTION_RESULT: {
            static unsigned long resultTimer = 0;
            static bool animationStarted = false;

            if(!animationStarted){
                resultTimer = millis();
                animationStarted = true;
                return;
            }

            if(millis() - resultTimer > 1000){

                if(is_transaction_complete){
                    update_transaction_result(true);
                } else {
                    update_transaction_result(false);
                }

                pumpStateTimer = millis();
                currentPumpState = PUMP_DELAY;
                pumpDelay = 2000;
                pumpDelayNextCurrentPumpState = is_transaction_complete ? PUMP_FINISHED : PUMP_IDLE;

                animationStarted = false;
            }

            break;
        }
        case PUMP_FINISHED: {
            if(previousPumpState != PUMP_FINISHED) {

                float totalPrice = price_per_liter * liters;
                load_thank_you_screen();

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
            if(previousPumpState != PUMP_HS){
                if(should_broadcast_HS){
                    broadcastPumpStatus("Hors_Service");
                }
                load_hs_screen();
                previousPumpState = PUMP_HS;
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