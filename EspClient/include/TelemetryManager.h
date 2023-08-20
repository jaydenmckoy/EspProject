#ifndef TELEMETRY_MANAGER_H
#define TELEMETRY_MANAGER_H

#include "main.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include "ImageTransferHandler.h"

#define TELEMETRY_PACKET_SIZE_MAX 1000


#pragma pack(1)
struct DataPacket_t
{
   size_t   size;
   uint8_t *data;
};

#pragma pack(1)
struct TelemetryPacket_t
{
   uint16_t apid;
   uint8_t  data[TELEMETRY_PACKET_SIZE_MAX];
};

enum eTelemetryApid
{
   PING_TM_APID,
   TAKE_IMAGE_TM_APID,
};

class TelemetryManager
{
   public:
      TelemetryManager(ImageTransferHandler *imgHandler);
      ~TelemetryManager();
      void AddPacket(const DataPacket_t &pkt);
      void Task(void);
   private:
      void ProcessTelemetry(const TelemetryPacket_t &tm_pkt);

      std::queue<TelemetryPacket_t> tmQueue;
      std::mutex queueMutex;
      std::condition_variable queueNotEmpty;
      ImageTransferHandler *imageHandler;
};


#endif