// TelemetryManager.h
#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

#include "Telemetry.h"

bool SendTelemetryPacket(TelemetryPacket_t *tmpkt);
bool SendTelemetry(eTelemetryApid apid, uint8_t * buf, size_t size);

#endif