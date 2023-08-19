#include "WebsocketClient.h"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char **argv)
{
   printf("EspClient started.\n");

   WebsocketClient websocketClient;
   std::thread websocketThread(&WebsocketClient::runWebSocket,
                               &websocketClient);
   websocketClient.ping();

   // Wait for the WebSocket client thread to finish (optional)
   websocketThread.join();

   return 0;
}
