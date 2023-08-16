// websocket.h
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncWebSocket.h>


bool SendClientData(uint8_t * data, size_t size);
bool ClientConnected(void);
void onWebSocketEvent(
   AsyncWebSocket        *wsServer,
   AsyncWebSocketClient  *client,
   AwsEventType           type,
   void                  *arg,
   uint8_t               *data,
   size_t                 len
   );
void WebsocketSetup(void);

#endif // WEBSOCKET_H
