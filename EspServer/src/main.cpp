#include "Camera.h"
#include "CommandHandler.h"
#include "Websocket.h"
#include <Arduino.h>

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

   // Setup end
   Serial.println("\nSetup complete.\n");
}

void loop()
{
   // main();
   delay(1000);
}