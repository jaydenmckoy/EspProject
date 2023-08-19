#include "CommandHandler.h"

CommandHandler::CommandHandler(WebsocketClient *client) : wsClient(client)
{

}

void CommandHandler::SendCommand(eCommandApid apid)
{
   // Format command packet
   CommandPacket_t cmd;
   cmd.apid = apid;
   switch(apid)
   {
      case PING_CMD_APID:
      for (uint8_t i = 0; i < COMMAND_PACKET_MAX_SIZE; i++) {
         cmd.data[i] = i + 1;
      }
         break;
      case TAKE_IMAGE_CMD_APID:
         break;
      default:
         printf("SendCommand error: unknown APID = %d\n",apid);
         return;
   }

   // Send websocket data
   wsClient->SendServerData((uint8_t *) &cmd, sizeof(CommandPacket_t));
}