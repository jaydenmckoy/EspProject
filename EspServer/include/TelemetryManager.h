// TelemetryManager.h
#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

#include "Telemetry.h"


class TelemetryManager
{
   public:
      static TelemetryManager& GetInstance(void);
      bool SendTelemetryPacket(TelemetryPacket_t *tmpkt);
      bool SendTelemetry(eTelemetryApid apid, uint8_t * buf, size_t size);
   private:
      // Private constructor to prevent direct instantiation
      TelemetryManager(void) {};
      // Private copy constructor and assignment operator to prevent copies
      TelemetryManager(const TelemetryManager&) = delete;
      TelemetryManager& operator=(const TelemetryManager&) = delete;

      // Private destructor to prevent deletion through pointers
      ~TelemetryManager() {
         // Cleanup code, if needed
      }
};


#endif