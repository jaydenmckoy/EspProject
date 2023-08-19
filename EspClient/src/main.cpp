#include <iostream>
#include <chrono>
#include <thread>
#include "WebsocketClient.h"

int main(int argc, char **argv) {
    printf("EspClient started.\n");

    WebsocketClient websocketClient;
    std::thread websocketThread(&WebsocketClient::runWebSocket,&websocketClient);
    websocketClient.ping();

   // Wait for the WebSocket client thread to finish (optional)
    websocketThread.join();

    return 0;
}
