#include "WebsocketClient.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "CommandHandler.h"

int main(int argc, char **argv)
{
   printf("EspClient started.\n");

   // Initialize and start WebsocketClient
   WebsocketClient websocketClient;
   std::thread websocketThread(&WebsocketClient::RunWebSocket,
                               &websocketClient);
   // Initialize CommandHandler
   CommandHandler cmdHandler(&websocketClient);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cmdHandler.SendCommand(PING_CMD_APID);

   // Wait for the WebSocket client thread to finish (optional)
   websocketThread.join();

   return 0;
}
