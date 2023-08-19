// websocket.cpp
#include "Websocket.h"
#include "Camera.h"
#include "CommandHandler.h"

const char* ap_ssid = "ESP32-CAM";
const char* ap_password = "esp32cam!";

AsyncWebServer wsServer(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient *gClient = NULL;

extern std::queue<DataPacket_t> commandQueue;

bool SendClientData(uint8_t * data, size_t size)
{
  if(gClient == NULL)
  {
    Serial.println("Client disconnected.");
    return false;
  }

  gClient->binary(data, size);
  return true;
}

bool ClientConnected(void)
{
  return gClient != NULL;
}

void onWebSocketEvent(AsyncWebSocket *wsServer, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  Serial.print("\nWebSocketEvent: ");
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.println("Client connected");
      gClient = client;
      break;
    case WS_EVT_DISCONNECT:
      Serial.println("Client disconnected");
      gClient = NULL;
      break;
    case WS_EVT_DATA:
      Serial.println("Client data received");
      // uint8_t someData[] = {1,2,3,4};
      // client->binary(someData,4);
      // delay(100);
      // SendClientData(data,4);
      DataPacket_t pkt = {0};
      pkt.len = len;
      pkt.data = data;
      commandQueue.push(pkt);
      break;
  }
}

void WebsocketSetup(void)
{
  // Initialize WiFi in SoftAP mode
  Serial.println("Setting AP (Access Point)…");
  WiFi.softAP(ap_ssid, ap_password);
  // Print the IP address of the ESP32-CAM
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  // Set up WebSocket event handler
  ws.onEvent(onWebSocketEvent);
  // Add WebSocket handler to wsServer
  wsServer.addHandler(&ws);
  // Start the wsServer
  wsServer.begin();
}
