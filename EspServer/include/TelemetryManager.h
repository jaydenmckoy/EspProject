// TelemetryManager.h
#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

// #include "Telemetry.h"
#include <Arduino.h>

#define TELEMETRY_PACKET_DATA_MAX_SIZE 1000

#pragma pack(1)
struct TelemetryPacket_t
{
   uint16_t  apid;
   uint32_t  length;
   uint8_t   data[TELEMETRY_PACKET_DATA_MAX_SIZE];
};

#pragma pack(1)
struct TelemetryPacketHeader_t
{
   uint16_t  apid;
   uint32_t  length;
};
#define TELEMETRY_PACKET_MAX_SIZE sizeof(TelemetryPacketHeader_t) + TELEMETRY_PACKET_DATA_MAX_SIZE

enum eTelemetryApid
{
   PING_TM_APID,
   TAKE_IMAGE_TM_APID,
   FILE_TX_START_TM_APID,
   FILE_TX_END_TM_APID,
   GET_FILE_TM_APID,
};

class TelemetryManager
{
   public:
      static TelemetryManager& GetInstance(void);
      void CreatePacket(const eTelemetryApid apid);
      void AddParameter(const uint8_t * param, const uint32_t paramLen);
      bool SendTelemetryPacket(void);
      bool SendTelemetryPacket(TelemetryPacket_t *tmpkt);
      bool SendTelemetry(eTelemetryApid apid, uint8_t * buf, size_t size);
   private:
      /********* Private Methods **********/
      // Private constructor to prevent direct instantiation
      TelemetryManager(void) {};
      // Private copy constructor and assignment operator to prevent copies
      TelemetryManager(const TelemetryManager&) = delete;
      TelemetryManager& operator=(const TelemetryManager&) = delete;

      // Private destructor to prevent deletion through pointers
      ~TelemetryManager() {
         // Cleanup code, if needed
      }

      /********* Private Variables **********/
      TelemetryPacketHeader_t pktHeader;
      uint8_t                 pktBuf[TELEMETRY_PACKET_MAX_SIZE];
};


#endif