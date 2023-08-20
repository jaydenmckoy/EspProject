#include "TelemetryManager.h"


TelemetryManager::TelemetryManager(ImageTransferHandler *imgHandler) : imageHandler(imgHandler)
{
}

TelemetryManager::~TelemetryManager()
{
}

void TelemetryManager::AddPacket(const DataPacket_t &pkt)
{
   if(pkt.size > sizeof(TelemetryPacket_t))
   {
      printf("TelemetryManager::AddPacket: data packet too large to unpack into telemetry packet (%ld > %ld)\n",pkt.size,sizeof(TelemetryPacket_t));
      return;
   }
   // printf("TelemetryManager::AddPacket: ");
   std::lock_guard<std::mutex> lock(queueMutex);
   TelemetryPacket_t tmpkt;
   memcpy(&tmpkt,pkt.data,TELEMETRY_PACKET_SIZE_MAX);
   // printf("\n-pushing\n");
   tmQueue.push(tmpkt);
   // printf("packet pushed.\n");
   queueNotEmpty.notify_one();//queueNotEmpty.notify_one();
}

void TelemetryManager::Task(void)
{
   while (true)
   {
      // printf("TelemetryManager::Task: waiting on queueNotEmpty\n");
      std::unique_lock<std::mutex> lock(queueMutex);
      queueNotEmpty.wait(lock, [this] { return !tmQueue.empty(); });
      printf("TelemetryManager::Task: packet received.");
      const TelemetryPacket_t tm_pkt = tmQueue.front();
      tmQueue.pop();
      lock.unlock();

      // Process the data here
      ProcessTelemetry(tm_pkt);
   }
}

void TelemetryManager::ProcessTelemetry(const TelemetryPacket_t &tm_pkt)
{
   const uint16_t apid = tm_pkt.apid;
   switch (apid)
   {
      case PING_TM_APID:
      {
         printf("PING_TM_APID: ");
         int i;
         for(i=0; i < TELEMETRY_PACKET_SIZE_MAX; i++) printf("0x%02X ",tm_pkt.data[i]);
         printf("\n");
         break;
      }
      case TAKE_IMAGE_TM_APID:
      {
         printf("TAKE_IMAGE_TM_APID: ");
         ImagePacket_t imgPkt = {0};
         memcpy((uint8_t *) &imgPkt, tm_pkt.data, sizeof(ImagePacket_t));
         imageHandler->ProcessImageTelemetry(&imgPkt);
         break;
      }
      
      default:
      {
         printf("ProcessTelemetry error - invalid APID: %d\n",apid);
         break;
      }
   }
}