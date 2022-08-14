#include "manager.h"

//=======================================================================
void ManagerEntity::run()
{
  rec_port = PORT_SERVER;
  send_port = PORT_CLI;
  update = true;

  std::thread sendDiscoveryThread(&ManagerEntity::handleDiscoveryThread, this);
  std::thread sendMonitoringThread(&ManagerEntity::handleMonitoringThread, this);
  std::thread managementThread(&ManagerEntity::handleManagementThread, this);
  std::thread interfaceThread(&ManagerEntity::handleInterfaceThread, this); 
  std::thread receiveMessagesThread(&ManagerEntity::handleReceiveThread, this);
  std::thread IOThread(&ManagerEntity::handleIOThread, this); 

  sendDiscoveryThread.join();
  sendMonitoringThread.join();
  interfaceThread.join();
  managementThread.join();
  receiveMessagesThread.join();
  IOThread.join();
}

//=======================================================================
void ManagerEntity::handleReceiveThread(){
  struct sockaddr_in rep_addr;
  PACKET packet;
  PARTICIPANT p;
  std::map<std::string, PARTICIPANT>::iterator it;
  MessageManager messageManager; 
  messageManager.setSocket(rec_port, false);

  while (1){
    messageManager.receiveMessage(&rep_addr, &packet);
    
    pMap_mutex.lock();

    switch (packet.type){
      case SLEEP_MONITORING_PACKET:
        if (pMap.count(packet.hostname)){
          it = pMap.find(packet.hostname);
          it->second.discovery_count = 0;
          it->second.monitoring_count = 0;
          it->second.awake = packet.awake;
        }
        break;

      case SLEEP_DISCOVERY_PACKET:   
        if (pMap.count(packet.hostname)) {
          it = pMap.find(packet.hostname);
          it->second.discovery_count = 0;
          it->second.active = packet.active;
        }
        else{
          strcpy(p.name, packet.hostname);
          strcpy(p.ip_addr, packet.ip_addr);
          strcpy(p.mac_addr, packet.mac_addr);
          p.active = packet.active;
          p.awake = packet.awake;
          p.monitoring_count = 0;   
          p.discovery_count = 0;
          pMap.insert(std::pair<std::string, PARTICIPANT>(packet.hostname, p)); 
        }
        break;
      
      case EXIT_PACKET:   
        if (pMap.count(packet.hostname)){
          it = pMap.find(packet.hostname); 
          it->second.active = false;
          it->second.awake = false;
        }
          
      break;

      default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    }

    pMap_mutex.unlock();
  }

  messageManager.closeSocket();
}

//=======================================================================
void ManagerEntity::handleManagementThread(){
  while(1){
    pMap_mutex.lock();
    for (auto it = pMap.begin(); it != pMap.end(); ++it){
      if (it->second.monitoring_count >= MAX_MONITORING_COUNT){
        it->second.awake = false;
      }
      
      if (it->second.discovery_count >= MAX_DISCOVERY_COUNT) {
        it->second.active = false;  
      }
    }
    pMap_mutex.unlock();

    sleep(1);
  }
}

//=======================================================================
void ManagerEntity::handleMonitoringThread(){
  PACKET packet;
  packet.type = SLEEP_MONITORING_PACKET;
  packet.awake = false;
  packet.active = false;

  repeatMessage(packet, 1);
}

//=======================================================================
void ManagerEntity::handleDiscoveryThread(){
  PACKET packet;
  packet.type = SLEEP_DISCOVERY_PACKET;
  packet.awake = false;
  packet.active = false;

  repeatMessage(packet, 2);
}

//=======================================================================
void ManagerEntity::incrementCounters(int type){
  pMap_mutex.lock();
  for (auto it = pMap.begin(); it != pMap.end(); ++it){
    switch(type){
    case SLEEP_DISCOVERY_PACKET: 
      it->second.discovery_count++;
      break;

    case SLEEP_MONITORING_PACKET: 
      it->second.monitoring_count++;
      break;
    }
  }
  pMap_mutex.unlock();
}

//=======================================================================
void ManagerEntity::handleIOThread(){
  char command[10];
  char hostname[MSG_STR_LEN];

  while(true){
    char str[10];
    scanf("%s", str);

    if (strcmp(str, "c") == 0){
      update = false;
      fprintf(stderr, "\033[A\bType command: ");
      scanf(" %s %s", command, hostname);
    
      if (strcmp(command, "WAKEUP") == 0){
        std::map<std::string, PARTICIPANT>::iterator it; 
       
        pMap_mutex.lock();
        it = pMap.find(hostname);
        pMap_mutex.unlock();

        PACKET packet;
        packet.type = WAKEUP_PACKET;
        strcpy(packet.hostname, it->second.name);
        strcpy(packet.ip_addr, it->second.ip_addr);
        strcpy(packet.mac_addr, it->second.mac_addr);
        packet.awake = true;

        char command[30];

        sprintf(command, "wakeonlan %s", packet.mac_addr);
        system(command);
      }

      update = true;
    }
  }
}

//=======================================================================
void ManagerEntity::repeatMessage(PACKET packet, float interval){
  MessageManager messageManager; 
  messageManager.setSocket(send_port, true);

  while(true){      
    incrementCounters(packet.type);
    messageManager.sendMessage(packet);
    sleep(interval);
  }

  messageManager.closeSocket();
}

//=======================================================================
void ManagerEntity::sendMessage(PACKET packet){
  MessageManager messageManager; 
  messageManager.setSocket(send_port, true);
  messageManager.sendMessage(packet);
  messageManager.closeSocket();
}

//=======================================================================
void ManagerEntity::handleInterfaceThread(){
  while(true){
    if (update){
      std::cout << "\033[H\033[2J\033[3J" ;
      std::cout << "MANAGER\n\n";

      TextTable *table = new TextTable('-', '|', '+');

      table->add( "Hostname" );
      table->add( "MAC Address" );
      table->add( "IP Address" );
      table->add( "Status" );
      table->add( "Disc_Count" );
      table->add( "Mon_Count" );
      table->endOfRow();

      pMap_mutex.lock();
      for (auto it = pMap.begin(); it != pMap.end(); ++it){
        if (it->second.active){
          table->add(it->second.name);
          table->add(it->second.mac_addr);
          table->add(it->second.ip_addr);
          table->add(it->second.awake ? "awake" : "ASLEEP" );
          table->add(std::to_string(it->second.discovery_count));
          table->add(std::to_string(it->second.monitoring_count));
          table->endOfRow();
        }
      }
      pMap_mutex.unlock();

      table->setAlignment( 2, TextTable::Alignment::RIGHT );
      std::cout << *table;
      delete table;
      sleep(2);
    }
  }
}

//=======================================================================