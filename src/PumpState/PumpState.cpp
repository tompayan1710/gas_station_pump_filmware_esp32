#include "PumpState.h"
#include <Arduino.h>

#include "../Ui/Screen_Home.h"
#include "../Ui/Screen_Fuel_Selection.h"

// On initialise les variables
PumpState currentPumpState = PUMP_BOOT;
PumpState previousPumpState = PUMP_BOOT;
unsigned long pumpStateTimer = 0;

void HandlePumpState() {
    switch (currentPumpState) {
        case PUMP_IDLE:
            load_home_screen(); 
            Serial.println("Action: Affichage Accueil");
            break;

        case PUMP_SELECT_FUEL:
            load_fuel_selection_screen();
            Serial.println("Action: Affichage choix carburant");
            break;

        case PUMP_SELECT_AMOUNT:
            // load_keypad_screen();
            Serial.println("Action: Affichage clavier montant");
            break;

        case PUMP_WAITING_PAYMENT:
            // load_payment_screen();
            Serial.println("Action: Affichage 'Insérez carte'");
            break;

        case PUMP_WAITING_AUTH:
            // load_loading_screen("Validation banque...");
            Serial.println("Action: Envoi requête WebSocket au serveur");
            break;

        case PUMP_READY_TO_FUEL:
            // load_action_prompt("Décrochez le pistolet");
            Serial.println("Action: Autorisation pompe active");
            break;

        case PUMP_FUELING:
            // load_progress_screen();
            Serial.println("Action: Distribution... Comptage des impulsions");
            break;

        case PUMP_FINISHED:
            // load_summary_screen();
            Serial.println("Action: Transaction terminée");
            pumpStateTimer = millis(); 
            break;

        default:
            break;
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