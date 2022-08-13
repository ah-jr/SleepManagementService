#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "../messaging/messages.h"
#include <stdio.h>

void sendDiscoveryPacket(uint16_t send_port);
void sendParticipantAck(struct sockaddr_in send_addr);

#endif