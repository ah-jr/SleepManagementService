#include "participant.h"

//=======================================================================
void ParticipantEntity::run()
{
  status.awake = true;
  status.active = true;
  gethostname(status.name, MSG_STR_LEN);
  strcpy(status.ip_addr, "socket not created"); 
  getMacAddress(status.mac_addr);

  rec_port = PORT_CLI;

  std::thread receiveMessageThread(&ParticipantEntity::handleMessageThread, this); 
  std::thread interfaceThread(&ParticipantEntity::handleInterfaceThread, this); 
  std::thread IOThread(&ParticipantEntity::handleIOThread, this); 

  receiveMessageThread.join();  
  IOThread.join();  
}

//=======================================================================
void ParticipantEntity::handleMessageThread()
{
  PACKET packet; 
  MessageManager messageManager; 
  struct sockaddr_in rep_addr;
  messageManager.setSocket(rec_port, false);

  while (true){
    messageManager.receiveMessage(&rep_addr, &packet);
    rep_addr.sin_port = htons(PORT_SERVER);

    status_mutex.lock();

    gethostname(packet.hostname, MSG_STR_LEN);
    messageManager.getIpAddress(packet.ip_addr); 
    getMacAddress(packet.mac_addr);
    strcpy(status.name, packet.hostname);
    strcpy(status.ip_addr, packet.ip_addr);
    strcpy(status.mac_addr, packet.mac_addr);

    switch (packet.type){

      case SLEEP_DISCOVERY_PACKET:
        packet.active = status.active;
        if (status.active && status.awake)
          messageManager.replyMessage(rep_addr, packet);
        break;
        
      case SLEEP_MONITORING_PACKET:
        packet.awake = status.awake;
        if (status.active && status.awake)
          messageManager.replyMessage(rep_addr, packet);
        break;

      case WAKEUP_PACKET:
        status.awake = packet.awake;
        break;

      default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    }

    status_mutex.unlock();
  }

  messageManager.closeSocket();
}

//=======================================================================
void ParticipantEntity::handleIOThread()
{
  char command[30];

  while(true){
    std::cin >> command; 

    status_mutex.lock();

    if (strcmp(command, "sleep") == 0)
      status.awake = false;
    else if (strcmp(command, "wakeup") == 0)
      status.awake = true;
    else if (strcmp(command, "exit") == 0){
      status.active = false;
      PACKET packet; 
      MessageManager messageManager; 
      messageManager.setSocket(PORT_SERVER, true);

      packet.type = EXIT_PACKET;
      packet.active = false;
      packet.awake = false;
      strcpy(packet.hostname, status.name);
      strcpy(packet.ip_addr, status.ip_addr);
      strcpy(packet.mac_addr, status.mac_addr);

      messageManager.sendMessage(packet);
      messageManager.closeSocket();
      exit(1);
    }
     
    else if (strcmp(command, "enter") == 0)
      status.active = true;  

    status_mutex.unlock();
  }
}

//=======================================================================
void ParticipantEntity::getMacAddress(char* mac_addr){
  std::string str_mac;
  std::ifstream in(MAC_ADDR_PATH);
  std::getline(in, str_mac);

  strcpy(mac_addr, str_mac.c_str());
}

//=======================================================================
void ParticipantEntity::handleInterfaceThread(){
  while(true){
    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "CLIENT\n\n";

    TextTable *table = new TextTable('-', '|', '+');

    table->add( "Hostname" );
    table->add( "MAC Address" );
    table->add( "IP Address" );
    table->add( "Active" );
    table->add( "Awake" );
    table->endOfRow();

    status_mutex.lock();

    table->add( status.name );
    table->add( status.mac_addr);
    table->add( status.ip_addr );
    table->add( status.active ? "true" : "false" );    
    table->add( status.awake ? "true" : "false" );
    table->endOfRow();

    status_mutex.unlock();

    table->setAlignment(2, TextTable::Alignment::RIGHT );
    std::cout << *table;
    delete table;
    sleep(3);
  }
}

//=======================================================================
