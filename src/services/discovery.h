#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "../messaging/messages.h"
#include <stdio.h>

void handleDiscoveryPacket(uint16_t send_port, MessageManager messageManager);

#endif