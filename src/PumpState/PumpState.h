#pragma once

#include <Arduino.h>

enum PumpState {
    PUMP_BOOT,            // Initialisation du système
    PUMP_IDLE,            // Écran d'accueil "Bienvenue"
    PUMP_SELECT_PAYMENT,
    PUMP_SELECT_FUEL,     // Choix du type : Diesel, SP95, etc.
    PUMP_INSERT_CARD,   // insertion carte
    PUMP_WAITING_PIN, // Lecture de la carte / insertion PIN
    PUMP_VERIFY_PIN,
    PUMP_WAITING_AUTH,    // Communication avec le serveur C#
    PUMP_READY_TO_FUEL,   // Pistolet autorisé (électrovanne prête)
    PUMP_FUELING,         // Débit en cours (comptage impulsions)
    PUMP_TRANSACTION_COMPLETE,
    PUMP_TRANSACTION_RESULT,
    PUMP_FINISHED,         // Fin de transaction, affichage résumé
    PUMP_DELAY,
    PUMP_HS                //Desactiver par un employé
};

extern PumpState previousPumpState;
extern PumpState currentPumpState;
extern unsigned long pumpStateTimer;

// PUMP_DELAY
extern int pumpDelay;
extern PumpState pumpDelayNextCurrentPumpState;

extern String currentTransactionId;

extern float price_SP95;
extern float price_DIESEL;

extern const char* fuelType;
extern float price_per_liter;
extern float min_liter;
extern float min_amount;
extern float amount;



extern const char* paymentType;

extern bool is_transaction_complete;
extern bool should_broadcast_HS;

void HandlePumpState();
void HandlePumpLoopingState();