#ifndef MONITORING_H
#define MONITORING_H

#include "../messaging/messages.h"
#include <stdio.h>

void handleMonitoringPacket(uint16_t send_port, MessageManager messageManager);

#endif