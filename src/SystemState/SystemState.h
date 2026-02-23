#pragma once
#include <WebSocketsClient.h>

enum SystemState {
    SYS_BOOT,              // démarrage matériel
    SYS_WIFI_CONNECTING,   // connexion WiFi en cours
    SYS_WS_CONNECTING,     // connexion WebSocket en cours
    SYS_REGISTERING,       // envoi CONNECT_PUMP, attente réponse
    SYS_READY,             // système opérationnel
    SYS_ERROR              // erreur critique
};

extern SystemState previousSystemState;
extern SystemState currentSystemState;
extern unsigned long systemStateTimer;
void handleSystemState();