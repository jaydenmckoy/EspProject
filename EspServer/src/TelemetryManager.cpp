// TelemetryManager.h
#include "TelemetryManager.h"
#include "Websocket.h"

bool SendTelemetry(TelemetryPacket_t * tmpkt)
{
   return SendClientData((uint8_t *) tmpkt, sizeof(TelemetryPacket_t));
}