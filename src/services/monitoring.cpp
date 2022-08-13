#include "monitoring.h"

void handleMonitoringPacket(uint16_t send_port, MessageManager messageManager){
  PACKET packet;
  packet.type = SLEEP_MONITORING_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Monitoring message");
  packet.length = 0;

  while(true){
    messageManager.sendBroadcastMessage(packet, send_port);
    sleep(3);
  }
}