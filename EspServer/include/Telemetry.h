// Telemetry.h
#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

#define COMMAND_PACKET_MAX_SIZE 3
#define TELEMETRY_PACKET_MAX_SIZE 1000

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

#pragma pack(1)
struct TelemetryPacket_t
{
   uint16_t apid;
   uint8_t data[TELEMETRY_PACKET_MAX_SIZE];
};

enum eCommandApid
{
   PING_CMD_APID,
   TAKE_TEST_IMAGE_CMD_APID,
   GET_TEST_IMAGE_CMD_APID,
   GET_FILE_CMD_APID,
   TAKE_IMAGE_CMD_APID,
};

enum eTelemetryApid
{
   PING_TM_APID,
   TAKE_IMAGE_TM_APID,
   FILE_TX_START_TM_APID,
   FILE_TX_END_TM_APID,
   GET_FILE_TM_APID,
};

#endif