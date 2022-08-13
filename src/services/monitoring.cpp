#include "monitoring.h"

void sendMonitoringPacket(uint16_t send_port){
  PACKET packet;

  packet.type = SLEEP_MONITORING_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Monitoring message");
  packet.length = 0;

  while(true){
    sendBroadcastMessage(packet, send_port);
    sleep(3);
  }
}