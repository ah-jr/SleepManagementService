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
  std::set<PARTICIPANT>::iterator it;
  MessageManager messageManager; 
  messageManager.setSocket(rec_port, false);

  while (1){
    messageManager.receiveMessage(&rep_addr, &packet);
    
    PARTICIPANT p;
    strcpy(p.name, packet.hostname);
    strcpy(p.mac_addr, packet.mac_addr);
    strcpy(p.ip_addr, packet.ip_addr);

    pSet_mutex.lock();

    switch (packet.type){
      case SLEEP_MONITORING_PACKET:
        if (pSet.count(p)){
          p.discovery_count = 0;
          p.awake = packet.awake;
          p.monitoring_count = 0;

          pSet.erase(p); 
          pSet.insert(p); 
        }

        break;

      case SLEEP_DISCOVERY_PACKET:   
        it = pSet.find(p);

        if (pSet.count(*it)) {
          p.monitoring_count = it->monitoring_count;
          pSet.erase(*it); 
        }
        else
          p.monitoring_count = 0;      

        p.active = packet.active;
        p.discovery_count = 0;
        pSet.insert(p); 
        break;

      default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    }

    pSet_mutex.unlock();
  }

  messageManager.closeSocket();
}

//=======================================================================
void ManagerEntity::handleManagementThread(){
  while(1){
    pSet_mutex.lock();
    for (auto it = pSet.begin(); it != pSet.end(); ++it){
      if ((*it).monitoring_count >= MAX_MONITORING_COUNT){
        PARTICIPANT p;

        strcpy(p.name, it->name);
        strcpy(p.ip_addr, it->ip_addr);
        strcpy(p.mac_addr, it->mac_addr);
        p.discovery_count = it->discovery_count;
        p.monitoring_count = it->monitoring_count;
        p.active = it->active;
        p.awake = false;

        pSet.erase(*it);
        pSet.insert(p);
      }
      
      if ((*it).discovery_count >= MAX_DISCOVERY_COUNT) {
        PARTICIPANT p;

        strcpy(p.name, it->name);
        strcpy(p.ip_addr, it->ip_addr);
        strcpy(p.mac_addr, it->mac_addr);
        p.discovery_count = it->discovery_count;
        p.monitoring_count = it->monitoring_count;
        p.active = false;
        p.awake = it->awake;

        pSet.erase(*it);
        pSet.insert(p);   	    
      }
    }
    pSet_mutex.unlock();

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
  pSet_mutex.lock();
  for (auto it = pSet.begin(); it != pSet.end(); ++it){
    PARTICIPANT p;

    strcpy(p.name, it->name);
    strcpy(p.ip_addr, it->ip_addr);
    strcpy(p.mac_addr, it->mac_addr);

    switch(type){
    case SLEEP_DISCOVERY_PACKET : 
      p.discovery_count = it->discovery_count + 1;
      p.monitoring_count = it->monitoring_count;
      break;

    case SLEEP_MONITORING_PACKET : 
      p.discovery_count = it->discovery_count;
      p.monitoring_count = it->monitoring_count + 1;
      break;
    }

    p.active = it->active;
    p.awake = it->awake;

    pSet.erase(*it);
    pSet.insert(p);  
  }

  pSet_mutex.unlock();
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
        std::set<PARTICIPANT>::iterator it;
        PARTICIPANT p;
        
        strcpy(p.name, hostname);
        
        pSet_mutex.lock();
        it = pSet.find(p);
        pSet_mutex.unlock();

        PACKET packet;
        packet.type = WAKEUP_PACKET;
        strcpy(packet.hostname, (*it).name);
        strcpy(packet.ip_addr, (*it).ip_addr);
        strcpy(packet.mac_addr, (*it).mac_addr);
        packet.awake = true;

        sendMessage(packet);
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

      pSet_mutex.lock();
      for (auto it = pSet.begin(); it != pSet.end(); ++it){
        if ((*it).active){
          table->add((*it).name);
          table->add((*it).mac_addr);
          table->add((*it).ip_addr);
          table->add((*it).awake ? "awake" : "ASLEEP" );
          table->add(std::to_string((*it).discovery_count));
          table->add(std::to_string((*it).monitoring_count));
          table->endOfRow();
        }
      }
      pSet_mutex.unlock();

      table->setAlignment( 2, TextTable::Alignment::RIGHT );
      std::cout << *table;
      delete table;
      sleep(2);
    }
  }
}

//=======================================================================