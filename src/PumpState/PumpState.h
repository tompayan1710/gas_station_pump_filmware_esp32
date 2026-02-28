#pragma once

#include <Arduino.h>

enum PumpState {
    PUMP_BOOT,            // Initialisation du système
    PUMP_IDLE,            // Écran d'accueil "Bienvenue"
    PUMP_SELECT_PAYMENT,
    PUMP_SELECT_FUEL,     // Choix du type : Diesel, SP95, etc.
    PUMP_SELECT_AMOUNT,   // Choix du montant max (ex: 100€)
    PUMP_WAITING_PAYMENT, // Lecture de la carte / insertion PIN
    PUMP_WAITING_AUTH,    // Communication avec le serveur C#
    PUMP_READY_TO_FUEL,   // Pistolet autorisé (électrovanne prête)
    PUMP_FUELING,         // Débit en cours (comptage impulsions)
    PUMP_TRANSACTION_COMPLETE,
    PUMP_FINISHED,         // Fin de transaction, affichage résumé
    PUMP_DELAY
};

extern PumpState previousPumpState;
extern PumpState currentPumpState;
extern unsigned long pumpStateTimer;

// PUMP_DELAY
extern int pumpDelay;
extern PumpState pumpDelayNextCurrentPumpState;

extern String currentTransactionId;

void HandlePumpState();
void HandlePumpLoopingState();