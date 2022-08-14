#include "manager.h"
#include "types.h"
#include <thread>

//=======================================================================
void ManagerEntity::run()
{
  rec_port = PORT_SERVER;
  send_port = PORT_CLI;

  std::thread sendDiscoveryThread(&ManagerEntity::handleDiscoveryThread, this);
  std::thread sendMonitoringThread(&ManagerEntity::handleMonitoringThread, this);
  std::thread interfaceThread(&ManagerEntity::handleInterfaceThread, this); 
  std::thread receiveMessagesThread(&ManagerEntity::handleReceiveThread, this);

  sendDiscoveryThread.join();
  sendMonitoringThread.join();
  interfaceThread.join();
  receiveMessagesThread.join();
}

//=======================================================================
void ManagerEntity::handleReceiveThread(){
  struct sockaddr_in rep_addr;
  PACKET packet;
  MessageManager messageManager; 
  messageManager.setSocket(rec_port, false);

  while (1){
    messageManager.receiveMessage(0, &rep_addr, &packet);
    
    PARTICIPANT p;
    strcpy(p.name, "Teste");
    strcpy(p.mac_addr, "");
    strcpy(p.ip_addr, std::to_string(rep_addr.sin_addr.s_addr).c_str());
    p.status = true;

    switch (packet.type){
      case SLEEP_MONITORING_PACKET:
        pSet.insert(p); 

        // for (auto it = pSet.begin(); it != pSet.end(); ++it)
        //   fprintf(stderr, "%s\n", (*it).name);

        break;
      case SLEEP_DISCOVERY_PACKET:
        break;
      default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    }
  }

  messageManager.closeSocket();
}

//=======================================================================
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

//=======================================================================
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

//=======================================================================
void ManagerEntity::handleInterfaceThread(){

}

//=======================================================================