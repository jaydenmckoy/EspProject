#include "main.h"
#include "Camera.h"
#include "CommandHandler.h"
#include "FileManager.h"
#include "Websocket.h"


/*
sudo chmod a+rw /dev/ttyUSB0
 */

void setup()
{
   // Main Setup
   Serial.begin(115200);
   Serial.println("\n\n\nBeginning Setup.\n");
   // Task setup
   CommandHandlerSetup();
   CameraSetup();
   WebsocketSetup();

   FileManager::Setup();

   // Setup end
   Serial.println("\nSetup complete.\n");
}

void loop()
{
   FileManager::Task();
   delay(10);
}