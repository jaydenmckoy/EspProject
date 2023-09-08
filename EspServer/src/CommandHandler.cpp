// CommandHandler.cpp
#include "CommandHandler.h"
#include "Camera.h"
#include "FileManager.h"
#include "Telemetry.h"
#include "TelemetryManager.h"

TaskHandle_t commandTask;
std::queue<DataPacket_t> commandQueue;

void HandleCommand(CommandPacket_t *cmd)
{
   // Get instance of TelemetryManager
   TelemetryManager& tmMgr = TelemetryManager::GetInstance();

   eCommandApid apid = (eCommandApid)cmd->apid;
   TelemetryPacket_t tm = {0};
   bool sendStatus = false;
   switch (apid) {
   case PING_CMD_APID:
      Serial.println("PING_CMD_APID received.");
      tmMgr.CreatePacket(PING_TM_APID);
      int i;
      for(i=0; i < 8; i++)
      {
         tmMgr.AddParameter(&cmd->data[i],1);
      }
      Serial.println("Delaying");
      delay(500);
      Serial.println("Sending telemetry");
      sendStatus = tmMgr.SendTelemetryPacket();
      Serial.printf("Ping success: %d\n", sendStatus);
      break;
   case TAKE_TEST_IMAGE_CMD_APID:
      Serial.println("TAKE_TEST_IMAGE_CMD_APID received.");
      TakeImage();
      break;
   case GET_TEST_IMAGE_CMD_APID:
      Serial.println("GET_TEST_IMAGE_CMD_APID received.");
      GetImage();
      break;
   case GET_FILE_CMD_APID:
      Serial.println("GET_FILE_CMD_APID received.");
      FileManager::GetFile();
      break;
   case TAKE_IMAGE_CMD_APID:
      Serial.println("TAKE_IMAGE_CMD_APID received.");
      TakeImage();
      break;

   default:
      Serial.printf("Unknown command received (apid=%d).\n", apid);
      break;
   }
}

void Task(void *param)
{
   while (true) {
      // Check for commands
      if (!commandQueue.empty()) {
         // Pop DataPacket
         DataPacket_t pkt = commandQueue.front();
         commandQueue.pop();
         // Unpacket data into CommandPacket
         CommandPacket_t cmd = {0};
         memcpy(&cmd, pkt.data, pkt.len);
         // Debugging
         int i;
         Serial.println("Command received.");
         // DataPacket
         Serial.printf("DataPacket:\n");
         Serial.printf("-pkt.len: %d\n", pkt.len);
         Serial.printf("-pkt.data: ");
         for (i = 0; i < pkt.len; i++)
            Serial.printf("0x%02X ", pkt.data[i]);
         Serial.println();
         // CommandPacket
         Serial.printf("CommandPacket:\n");
         Serial.printf("-APID: %d\n", cmd.apid);
         Serial.printf("-parameters: ");
         for (i = 0; i < COMMAND_PACKET_MAX_SIZE; i++)
            Serial.printf("0x%02X ", cmd.data[i]);
         Serial.println();
         // Handle Command
         HandleCommand(&cmd);
      }
      // Delay for a short time to avoid busy-waiting
      delay(10);
   }
}

void CommandHandlerSetup(void)
{
   Serial.print("Setting up CommandHandler: ");
   // Create the command handling task
   xTaskCreatePinnedToCore(Task,             // Task function
                           "CommandHandler", // Task name
                           8192,             // Stack size (bytes)
                           NULL,             // Task parameter
                           1,                // Priority
                           &commandTask,     // Task handle
                           0                 // Run on core 0
   );
   Serial.println("Complete.");
}