#include "discovery.h"

void sendDiscoveryPacket(){
  PACKET packet;

  packet.type = SLEEP_DISCOVERY_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "oi\0");
  packet.length = 0;

  while(TRUE){
    sendBroadcastMessage(packet);
    sleep(3);
  }
}

void sendParticipantAck(struct sockaddr_in cli_addr){
  PACKET packet;

  packet.type = SLEEP_DISCOVERY_PACKET_ACK;
  packet.seqn = 0;
  strcpy(packet.payload, "oi\0");
  packet.length = 0;

  sendMessage(packet, cli_addr);
}