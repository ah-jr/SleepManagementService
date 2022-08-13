#include "discovery.h"

void sendDiscoveryPacket(uint16_t send_port){
  PACKET packet;

  packet.type = SLEEP_DISCOVERY_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "oi\0");
  packet.length = 0;

  while(true){
    sendBroadcastMessage(packet, send_port);
    sleep(3);
  }
}

void sendParticipantAck(struct sockaddr_in send_addr){
  PACKET packet;

  packet.type = SLEEP_DISCOVERY_PACKET_ACK;
  packet.seqn = 0;
  strcpy(packet.payload, "ack\0");
  packet.length = 0;

  sendMessage(packet, send_addr);
}