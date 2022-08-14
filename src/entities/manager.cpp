#include "manager.h"

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
    messageManager.receiveMessage(&rep_addr, &packet);
    
    PARTICIPANT p;
    strcpy(p.name, "Teste");
    strcpy(p.mac_addr, "");
    strcpy(p.ip_addr, std::to_string(rep_addr.sin_addr.s_addr).c_str());

    pSet_mutex.lock();

    switch (packet.type){
      case SLEEP_MONITORING_PACKET:
        if (pSet.count(p)){
          pSet.erase(p); 
          p.status = (strcmp(packet.payload, "awaken") == 0);
          pSet.insert(p); 
        }
        break;

      case SLEEP_DISCOVERY_PACKET:   
        if (strcmp(packet.payload, "active") == 0)
          pSet.insert(p);
        else    
          pSet.erase(p); 
        break;

      default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    }

    pSet_mutex.unlock();
  }

  messageManager.closeSocket();
}

//=======================================================================
void ManagerEntity::handleMonitoringThread(){
  PACKET packet;
  packet.type = SLEEP_MONITORING_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Monitoring message");
  packet.length = 0;

  broadcastMessage(packet);
}

//=======================================================================
void ManagerEntity::handleDiscoveryThread(){
  PACKET packet;
  packet.type = SLEEP_DISCOVERY_PACKET;
  packet.seqn = 0;
  strcpy(packet.payload, "Discovery message");
  packet.length = 0;

  broadcastMessage(packet);
}

//=======================================================================
void ManagerEntity::broadcastMessage(PACKET packet){
  MessageManager messageManager; 
  messageManager.setSocket(send_port, true);

  while(true){
    messageManager.sendMessage(packet);
    sleep(1);
  }

  messageManager.closeSocket();
}

//=======================================================================
void ManagerEntity::handleInterfaceThread(){
  while(true){
    std::cout << "\033[H\033[2J\033[3J" ;
    TextTable *table = new TextTable('-', '|', '+');

    table->add( "Hostname" );
    table->add( "MAC Address" );
    table->add( "IP Address" );
    table->add( "Status" );
    table->endOfRow();

    pSet_mutex.lock();
    for (auto it = pSet.begin(); it != pSet.end(); ++it){
      table->add( (*it).name );
      table->add( (*it).mac_addr);
      table->add( (*it).ip_addr );
      table->add( (*it).status ? "awake" : "ASLEEP" );
      table->endOfRow();
    }
    pSet_mutex.unlock();

    table->setAlignment( 2, TextTable::Alignment::RIGHT );
    std::cout << *table;
    delete table;
    sleep(3);
  }
}

//=======================================================================