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

void TelemetryManager::CreatePacket(const eTelemetryApid apid)
{
   memset(pktBuf,0,TELEMETRY_PACKET_MAX_SIZE);
   pktHeader.apid = apid;
   pktHeader.length = 0;
}
void TelemetryManager::AddParameter(const uint8_t * param, const uint32_t paramLen)
{
   memcpy(&pktBuf[sizeof(pktHeader) + pktHeader.length], param, paramLen);
   pktHeader.length += paramLen;
}

bool TelemetryManager::SendTelemetryPacket(void)
{
   memcpy(pktBuf, (uint8_t *) &pktHeader, sizeof(pktHeader));
   return SendClientData(pktBuf, sizeof(pktHeader) + pktHeader.length);
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