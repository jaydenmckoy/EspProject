#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <Arduino.h>
#include <queue>
#include "Telemetry.h"

extern std::queue<DataPacket_t> commandQueue;

void CommandHandlerSetup(void);

#endif
