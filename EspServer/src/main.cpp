#include "main.h"
#include "Camera.h"
#include "CommandHandler.h"
#include "FileManager.h"
#include "Websocket.h"


/*
sudo chmod a+rw /dev/ttyUSB0
 */

void GetVitals(void)
{
   // Get total heap size
   size_t heapSize = ESP.getHeapSize();
   Serial.printf("Total Heap Size: %u bytes\n", heapSize);

   // Get free heap size
   size_t freeHeap = ESP.getFreeHeap();
   Serial.printf("Free Heap Size: %u bytes\n", freeHeap);

   // Get minimum free heap size
   size_t minFreeHeap = ESP.getMinFreeHeap();
   Serial.printf("Minimum Free Heap Size: %u bytes\n", minFreeHeap);

   // Get total PSRAM size (if available)
   size_t psramSize = ESP.getPsramSize();
   if (psramSize > 0) {
      Serial.printf("Total PSRAM Size: %u bytes\n", psramSize);
   } else {
      Serial.println("No PSRAM available");
   }
}

void setup()
{
   // Main Setup
   Serial.begin(115200);
   Serial.println("\n\n\nBeginning Setup.\n");

   GetVitals();
   
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