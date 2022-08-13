#include "discovery.h"

void sendDiscoveryPacket(uint16_t send_port){
  PACKET packet;

  packet.type = SLEEP_DISCOVERY_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Discovery message");
  packet.length = 0;

  while(true){
    sendBroadcastMessage(packet, send_port);
    sleep(3);
  }
}

