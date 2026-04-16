#include "PumpState.h"

#include <Arduino.h>

#include "../Config.h"
#include "../SystemState/WebSocketManager.h"
#include "./KeypadManager.h"

#include "../Ui/Screen_Fuel_Selection.h"
#include "../Ui/Screen_HS.h"
#include "../Ui/Screen_Home.h"
#include "../Ui/Screen_Insert_Card.h"
#include "../Ui/Screen_PIN.h"
#include "../Ui/Screen_Payment_Type.h"
#include "../Ui/Screen_Pump_Filing.h"
#include "../Ui/Screen_Ready_To_Fuel.h"
#include "../Ui/Screen_Thank_You.h"
#include "../Ui/Screen_Transaction_Waiting_Auth.h"
#include "../Ui/Screen_Transaction_Waiting_Complete.h"
#include "../Ui/Screen_Verify_Pin.h"
#include "../Ui/Ui_Manager.h"

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

void resetPumpSession() {
    currentTransactionId = "";
    price_SP95 = -1.0;
    price_DIESEL = -1.0;
    fuelType = "";
    price_per_liter = 0.0;
    min_liter = -1.0;
    min_amount = -1.0;
    amount = -1.0;
    paymentType = "";
    is_transaction_complete = false;
    should_broadcast_HS = false;
}

void HandlePumpState() {
    static float liters = 0.0;
    static unsigned long lastFlowTime = 0;
    static unsigned long lastUpdateTime = 0;
    static bool fuelScreenLoaded = false;

    switch (currentPumpState) {
        case PUMP_IDLE: {
            if (previousPumpState != PUMP_IDLE) {
                resetPumpSession();
                fuelScreenLoaded = false;
                pumpStateTimer = 0;

                load_home_screen();
                pumpStateTimer = millis();
                previousPumpState = PUMP_IDLE;
            }
            break;
        }

        case PUMP_SELECT_FUEL: {
            if (previousPumpState != PUMP_SELECT_FUEL) {
                requestFuelPrices();
                pumpStateTimer = millis();
                previousPumpState = PUMP_SELECT_FUEL;
            }

            if (price_SP95 > 0 && price_DIESEL > 0 && !fuelScreenLoaded) {
                Serial.println("Prix OK -> affichage");
                load_fuel_selection_screen();
                fuelScreenLoaded = true;
                previousPumpState = PUMP_SELECT_FUEL;
                break;
            }

            if ((price_SP95 <= 0 || price_DIESEL <= 0) && millis() - pumpStateTimer > FUEL_PRICE_TIMEOUT_MS) {
                Serial.println("Timeout prix carburant");
                should_broadcast_HS = true;
                currentPumpState = PUMP_HS;
            }
            break;
        }

        case PUMP_SELECT_PAYMENT: {
            if (previousPumpState != PUMP_SELECT_PAYMENT) {
                load_payment_type_screen();
                Serial.println("Affichage choix carte");
                previousPumpState = PUMP_SELECT_PAYMENT;
            }
            break;
        }

        case PUMP_INSERT_CARD: {
            if (previousPumpState != PUMP_INSERT_CARD) {
                load_insert_card_screen();
                Serial.println("Affichage PUMP_INSERT_CARD");

                pumpStateTimer = millis();
                previousPumpState = PUMP_INSERT_CARD;
            }

            if (millis() - pumpStateTimer > INSERT_CARD_DELAY_MS) {
                currentPumpState = PUMP_WAITING_PIN;
            }
            break;
        }

        case PUMP_WAITING_PIN: {
            static char pin_enter[5] = "";
            static int number_pin_enter = 0;

            if (previousPumpState != PUMP_WAITING_PIN) {
                load_pin_screen();
                Serial.println("Action: Affichage PIN");

                number_pin_enter = 0;
                pin_enter[0] = '\0';
                lv_label_set_text(pin_label, "_   _   _   _");

                pumpStateTimer = millis();
                previousPumpState = PUMP_WAITING_PIN;
            }

            char key = keypad.getKey();

            if (key) {
                if (key == '*') {
                    number_pin_enter = 0;
                    pin_enter[0] = '\0';
                    lv_label_set_text(pin_label, "_   _   _   _");
                    return;
                }

                if (key >= '0' && key <= '9' && number_pin_enter < 4) {
                    char buffer[32];

                    pin_enter[number_pin_enter] = key;
                    number_pin_enter++;
                    pin_enter[number_pin_enter] = '\0';

                    if (number_pin_enter == 1) snprintf(buffer, sizeof(buffer), "%c   _   _   _", pin_enter[0]);
                    if (number_pin_enter == 2) snprintf(buffer, sizeof(buffer), "%c   %c   _   _", pin_enter[0], pin_enter[1]);
                    if (number_pin_enter == 3) snprintf(buffer, sizeof(buffer), "%c   %c   %c   _", pin_enter[0], pin_enter[1], pin_enter[2]);
                    if (number_pin_enter == 4) snprintf(buffer, sizeof(buffer), "%c   %c   %c   %c", pin_enter[0], pin_enter[1], pin_enter[2], pin_enter[3]);

                    lv_label_set_text(pin_label, buffer);

                    if (number_pin_enter == 4) {
                        Serial.print("PIN entered: ");
                        Serial.println(pin_enter);
                        currentPumpState = PUMP_VERIFY_PIN;
                    }
                }
            }
            break;
        }

        case PUMP_VERIFY_PIN: {
            static unsigned long verifyTimer = 0;
            static bool verifyStarted = false;
            static bool resultShown = false;
            // TODO: Replace this local simulation with a real PIN validation request to the backend.
            static bool isPinValid = false;
            static int pinAttempts = 0;

            if (previousPumpState != PUMP_VERIFY_PIN) {
                load_verify_pin();

                verifyTimer = millis();
                verifyStarted = true;
                resultShown = false;
                previousPumpState = PUMP_VERIFY_PIN;
            }

            if (verifyStarted && !resultShown && millis() - verifyTimer > PIN_VERIFY_DELAY_MS) {
                // TODO: Set this value from the backend response instead of forcing a local demo value.
                isPinValid = true;

                hide_verify_spinner();

                if (isPinValid) {
                    update_verify_pin_text("CODE BON", true);
                } else {
                    update_verify_pin_text("CODE INVALIDE", false);
                }

                resultShown = true;
                verifyTimer = millis();
            }

            if (resultShown && millis() - verifyTimer > PIN_RESULT_DELAY_MS) {
                if (isPinValid) {
                    // TODO: Move to the next state only after the backend confirms the PIN is valid.
                    pinAttempts = 0;
                    currentPumpState = PUMP_WAITING_AUTH;
                } else {
                    // TODO: Use the real backend error result here to handle invalid PIN attempts.
                    pinAttempts++;

                    if (pinAttempts >= 3) {
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

        case PUMP_WAITING_AUTH: {
            if (previousPumpState != PUMP_WAITING_AUTH) {
                Serial.println("PUMP_WAITING_AUTH");
                requestCarburantConfig();
                pumpStateTimer = 0;
                previousPumpState = PUMP_WAITING_AUTH;
            }

            if (min_liter > 0 && pumpStateTimer == 0) {
                min_amount = min_liter * price_per_liter;
                Serial.println("sendStartTransactionAuthPacket");
                sendStartTransactionAuthPacket(fuelType, paymentType, min_amount);
                load_waiting_auth_screen();
                pumpStateTimer = millis();
            }
            break;
        }

        case PUMP_READY_TO_FUEL: {
            if (previousPumpState != PUMP_READY_TO_FUEL) {
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

        case PUMP_FUELING: {
            if (previousPumpState != PUMP_FUELING) {
                load_progress_screen();
                liters = 0.0;
                lastFlowTime = millis();
                lastUpdateTime = millis();
                previousPumpState = PUMP_FUELING;
            }

            Serial.println(digitalRead(DIS));

            if (digitalRead(DIS) == HIGH) {
                ledcWrite(PWM_CHANNEL_R, 150);
                ledcWrite(PWM_CHANNEL_L, 0);

                lastFlowTime = millis();

                if (millis() - lastUpdateTime > FUELING_UPDATE_INTERVAL_MS) {
                    char buffer[32];
                    float price_ht;
                    float price_ttc;
                    int percent;

                    lastUpdateTime = millis();
                    liters += 0.05;

                    price_ht = liters * price_per_liter;
                    price_ttc = price_ht * 1.20;

                    snprintf(buffer, sizeof(buffer), "%.2f L", liters);
                    lv_label_set_text(progress_label, buffer);

                    snprintf(buffer, sizeof(buffer), "HT: %.2f EUR", price_ht);
                    lv_label_set_text(price_ht_label, buffer);

                    snprintf(buffer, sizeof(buffer), "TTC: %.2f EUR", price_ttc);
                    lv_label_set_text(price_ttc_label, buffer);

                    percent = (int)(price_ht * 2);
                    if (percent > 100) percent = 100;

                    lv_bar_set_value(progress_bar, percent, LV_ANIM_OFF);

                    if (price_ht >= min_amount) {
                        lv_label_set_text(min_label, "Minimum atteint");
                        lv_obj_set_style_text_color(min_label, COLOR_GREEN, 0);
                        lv_obj_set_style_text_color(progress_label, COLOR_GREEN, 0);
                        lv_obj_set_style_bg_color(progress_bar, COLOR_GREEN, LV_PART_INDICATOR);
                    } else {
                        lv_obj_set_style_bg_color(progress_bar, COLOR_RED, LV_PART_INDICATOR);
                    }
                }
            } else {
                ledcWrite(PWM_CHANNEL_R, 0);
                ledcWrite(PWM_CHANNEL_L, 0);
            }

            if (millis() - lastFlowTime > FUELING_STOP_TIMEOUT_MS) {
                currentPumpState = PUMP_TRANSACTION_COMPLETE;
            }
            break;
        }

        case PUMP_TRANSACTION_COMPLETE: {
            if (previousPumpState != PUMP_TRANSACTION_COMPLETE) {
                load_transaction_waiting_complete_screen(liters, price_per_liter, price_per_liter * liters);
                sendTransactionCompletePacket(currentTransactionId.c_str(), liters);
                previousPumpState = PUMP_TRANSACTION_COMPLETE;
            }
            break;
        }

        case PUMP_TRANSACTION_RESULT: {
            static unsigned long resultTimer = 0;
            static bool animationStarted = false;

            if (!animationStarted) {
                resultTimer = millis();
                animationStarted = true;
                return;
            }

            if (millis() - resultTimer > TRANSACTION_RESULT_DELAY_MS) {
                if (is_transaction_complete) {
                    update_transaction_result(true);
                } else {
                    update_transaction_result(false);
                }

                pumpStateTimer = millis();
                currentPumpState = PUMP_DELAY;
                pumpDelay = RESULT_TO_NEXT_STATE_DELAY_MS;
                pumpDelayNextCurrentPumpState = is_transaction_complete ? PUMP_FINISHED : PUMP_IDLE;
                animationStarted = false;
            }
            break;
        }

        case PUMP_FINISHED: {
            if (previousPumpState != PUMP_FINISHED) {
                load_thank_you_screen();
                pumpStateTimer = millis();
                previousPumpState = PUMP_FINISHED;
            }

            if (millis() - pumpStateTimer > FINISHED_SCREEN_DELAY_MS) {
                currentPumpState = PUMP_IDLE;
            }
            break;
        }

        case PUMP_DELAY: {
            if (previousPumpState != PUMP_DELAY) {
                pumpStateTimer = millis();
                previousPumpState = PUMP_DELAY;
            }

            if (millis() - pumpStateTimer > pumpDelay) {
                currentPumpState = pumpDelayNextCurrentPumpState;
            }
            break;
        }

        case PUMP_HS: {
            if (previousPumpState != PUMP_HS) {
                if (should_broadcast_HS) {
                    broadcastPumpStatus("Hors_Service");
                }
                load_hs_screen();
                previousPumpState = PUMP_HS;
            }
            break;
        }

        default:
            break;
    }
}

void HandlePumpLoopingState() {
    switch(currentPumpState) {
        case PUMP_FUELING:
            break;

        case PUMP_FINISHED:
            if (millis() - pumpStateTimer > FINISHED_SCREEN_DELAY_MS * 2) {
                currentPumpState = PUMP_IDLE;
            }
            break;

        default:
            break;
    }
}
