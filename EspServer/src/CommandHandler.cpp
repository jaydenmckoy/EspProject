// CommandHandler.cpp
#include "CommandHandler.h"

TaskHandle_t commandTask;
std::queue<DataPacket_t> commandQueue;

void HandleCommand(CommandPacket_t * cmd)
{
   eCommandApid apid = (eCommandApid) cmd->apid;
   switch (apid)
   {
      case PING_CMD_APID:
         Serial.println("TAKE_IMAGE_CMD_APID received.");
         break;
      case TAKE_IMAGE_CMD_APID:
         Serial.println("TAKE_IMAGE_CMD_APID received.");
         break;
      
      default:
         Serial.printf("Unknown command received (apid=%d).\n",apid);
         break;
   }
}

void Task(void *param) {
    while (true) {
      // Check for commands
        if (!commandQueue.empty()) {
            // Pop DataPacket
            DataPacket_t pkt = commandQueue.front();
            commandQueue.pop();
            // Unpacket data into CommandPacket
            CommandPacket_t cmd = {0};
            memcpy(&cmd,pkt.data,pkt.len);
            // Debugging
            int i;
            Serial.println("Command received.");
            // DataPacket
            Serial.printf("DataPacket:\n");
            Serial.printf("-pkt.len: %d\n",pkt.len);
            Serial.printf("-pkt.data: ");
            for(i=0; i < pkt.len; i++) Serial.printf("0x%02X ",pkt.data[i]);
            Serial.println();
            // CommandPacket
            Serial.printf("CommandPacket:\n");
            Serial.printf("-APID: %d\n",cmd.apid);
            Serial.printf("-parameters: ");
            for(i=0; i < COMMAND_PACKET_MAX_SIZE; i++) Serial.printf("0x%02X ",cmd.data[i]);
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
    xTaskCreatePinnedToCore(
        Task,   // Task function
        "CommandHandler",     // Task name
        4096,                 // Stack size (bytes)
        NULL,                 // Task parameter
        1,                    // Priority
        &commandTask,         // Task handle
        0                     // Run on core 0
    );
   Serial.println("Complete.");
}