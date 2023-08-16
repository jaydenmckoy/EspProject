#include <Arduino.h>
#include "camera.h"
#include "websocket.h"
#include "CommandHandler.h"

/*
sudo chmod a+rw /dev/ttyUSB0
 */

void MainSetup(void)
{
  Serial.begin(115200);
}

void setup() {
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

int main(void)
{ 
  if(ClientConnected())
  {
    TakeImage();
  }
  delay(5000);
  return 0;
}

void loop()
{
  // main();
  delay(1000);
}