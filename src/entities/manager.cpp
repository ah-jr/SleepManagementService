#include "manager.h"
#include <thread>

void ManagerEntity::run()
{
  // Initialize variables
  rec_port = PORT_SERVER;
  send_port = PORT_CLI;

  // Start active services
  std::thread sendDiscoveryThread(&ManagerEntity::handleDiscoveryThread, this);
  std::thread sendMonitoringThread(&ManagerEntity::handleMonitoringThread, this);
  //std::thread managementThread(&handleManagement); 

  // Start passive services
  std::thread receiveMessagesThread(&ManagerEntity::handleReceiveThread, this);

  // Join Threads
  sendDiscoveryThread.join();
  sendMonitoringThread.join();
  receiveMessagesThread.join();
}

void ManagerEntity::handleReceiveThread(){
  struct sockaddr_in rep_addr;
  PACKET packet;
  MessageManager messageManager; 
  messageManager.setSocket(rec_port, false);

  while (1){
    memset(&packet, 0, sizeof(packet));
    memset(&rep_addr, 0, sizeof(rep_addr));
    messageManager.receiveMessage(0, &rep_addr, &packet);
    
    // PList participant;
    // strcpy(participant.name, "Teste");
    // participant.mac_addr =
    // participant.ip_addr = rep_addr.sin_addr.s_addr;
    // participant.status =            

    // switch (packet.type){
    //   case SLEEP_MONITORING_PACKET:
    //     //participantsVec.push_back(participant);
    //     break;
    //   default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    // }
  }

  messageManager.closeSocket();
}

void ManagerEntity::handleMonitoringThread(){
  MessageManager messageManager; 
  PACKET packet;
	messageManager.setSocket(send_port, true);
  packet.type = SLEEP_MONITORING_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Monitoring message");
  packet.length = 0;

  while(true){
    messageManager.sendBroadcastMessage(packet, send_port);
    sleep(1);
  }
  
  messageManager.closeSocket();
}

void ManagerEntity::handleDiscoveryThread(){
  MessageManager messageManager; 
  PACKET packet;
  messageManager.setSocket(send_port, true);
  packet.type = SLEEP_DISCOVERY_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Discovery message");
  packet.length = 0;

  while(true){
    messageManager.sendBroadcastMessage(packet, send_port);
    sleep(1);
  }

  messageManager.closeSocket();
}
