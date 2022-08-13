#include "discovery.h"

void handleDiscoveryPacket(uint16_t send_port, MessageManager messageManager){
  PACKET packet;
  packet.type = SLEEP_DISCOVERY_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Discovery message");
  packet.length = 0;

  while(true){
    messageManager.sendBroadcastMessage(packet, send_port);
    sleep(3);
  }
}

