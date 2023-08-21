#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "WebsocketClient.h"

#define COMMAND_PACKET_MAX_SIZE 3

enum eCommandApid
{
   PING_CMD_APID,
   TAKE_TEST_IMAGE_CMD_APID,
   TAKE_IMAGE_CMD_APID,
};

#pragma pack(1)
struct CommandPacket_t
{
   uint16_t apid;
   uint8_t data[COMMAND_PACKET_MAX_SIZE];
};

class CommandHandler
{
   public:
      CommandHandler(WebsocketClient *client);
      ~CommandHandler();
      void SendCommand(eCommandApid apid);
   private:
      WebsocketClient *wsClient;
};


#endif