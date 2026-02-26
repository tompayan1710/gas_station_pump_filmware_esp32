#pragma once

#include <WebSocketsClient.h>

extern WebSocketsClient webSocket;
extern String lastWebSocketMessage;

void initWebSocket();
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
void sendRegistrationPacket();

void sendStartTransactionAuthPacket(const char *fuelType, const char *paymentType, float amount);
void sendTransactionCompletePacket(const char *transactionId, float liters, float totalPrice);