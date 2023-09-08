// TelemetryManager.h
#include "TelemetryManager.h"
#include "Websocket.h"
#include <algorithm>


TelemetryManager& TelemetryManager::GetInstance(void)
{
   // Create the instance if it doesn't exist
   static TelemetryManager instance;
   return instance;
}


bool TelemetryManager::SendTelemetryPacket(TelemetryPacket_t * tmpkt)
{
   return SendClientData((uint8_t *) tmpkt, sizeof(TelemetryPacket_t));
}

bool TelemetryManager::SendTelemetry(eTelemetryApid apid, uint8_t * buf, size_t size)
{
   Serial.printf("Sending telemetry: %d\n",apid);
   // Initialize packet
   TelemetryPacket_t tm_pkt = {0};
   tm_pkt.apid = apid;
   // Send telemetry
   size_t bytesSent = 0;
   int numBytes;
   size_t pktSizeMax = TELEMETRY_PACKET_MAX_SIZE;
   while(bytesSent < size)
   {
      numBytes = std::min(size - bytesSent, pktSizeMax);
      memcpy(tm_pkt.data,&buf[bytesSent],numBytes);
      if(SendTelemetryPacket(&tm_pkt) == false)
      {
         Serial.println("Error sending telemetry. Aborting.");
         return false;
      }
      bytesSent += numBytes;
   }
   return true;
}