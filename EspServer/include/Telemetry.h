// Telemetry.h
#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

#define COMMAND_PACKET_MAX_SIZE 3

#pragma pack(1)
struct DataPacket_t
{
   size_t len;
   uint8_t *data;
};

#pragma pack(1)
struct CommandPacket_t
{
   uint16_t apid;
   uint8_t data[COMMAND_PACKET_MAX_SIZE];
};

enum eCommandApid
{
   PING_CMD_APID,
   TAKE_TEST_IMAGE_CMD_APID,
   GET_TEST_IMAGE_CMD_APID,
   GET_FILE_CMD_APID,
   TAKE_IMAGE_CMD_APID,
};


#endif