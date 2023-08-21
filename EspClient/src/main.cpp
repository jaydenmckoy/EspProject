#include "main.h"
#include "CommandHandler.h"
#include "TelemetryManager.h"
#include "WebsocketClient.h"
#include "ImageTransferHandler.h"

int main(int argc, char **argv)
{
   printf("EspClient started.\n");

   // Initialize ImageTransferHandler
   ImageTransferHandler imgHandler;
   // Initialize TelemetryManager
   TelemetryManager tmManager(&imgHandler);
   std::thread telemetryThread(&TelemetryManager::Task,&tmManager);
   // Initialize and start WebsocketClient
   WebsocketClient websocketClient(&tmManager);
   std::thread websocketThread(&WebsocketClient::RunWebSocket,&websocketClient);
   // Initialize CommandHandler
   CommandHandler cmdHandler(&websocketClient);
   std::this_thread::sleep_for(std::chrono::seconds(1));
   // cmdHandler.SendCommand(PING_CMD_APID);
   // cmdHandler.SendCommand(TAKE_IMAGE_CMD_APID);
   cmdHandler.SendCommand(TAKE_TEST_IMAGE_CMD_APID);

   // Wait for the WebSocket client thread to finish (optional)
   telemetryThread.join();
   websocketThread.join();

   return 0;
}
