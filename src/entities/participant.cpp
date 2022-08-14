#include "participant.h"

//=======================================================================
void ParticipantEntity::run()
{
    awake = true;
    active = true;
    rec_port = PORT_CLI;

    std::thread receiveMessageThread(&ParticipantEntity::handleMessageThread, this); 
    std::thread IOThread(&ParticipantEntity::handleIOThread, this); 

    receiveMessageThread.join();  
    IOThread.join();  
}

//=======================================================================
void ParticipantEntity::handleMessageThread()
{
  struct sockaddr_in rep_addr;
  PACKET packet; 
  MessageManager messageManager; 
  messageManager.setSocket(rec_port, false);

  while (true){
    messageManager.receiveMessage(&rep_addr, &packet);
    rep_addr.sin_port = htons(PORT_SERVER);
    
    switch (packet.type){

      case SLEEP_DISCOVERY_PACKET:
        active ? strcpy(packet.payload, "active") : strcpy(packet.payload, "inactive");
        messageManager.replyMessage(rep_addr, packet);
        break;
        
      case SLEEP_MONITORING_PACKET:
        awake ? strcpy(packet.payload, "awaken") : strcpy(packet.payload, "asleep");
        messageManager.replyMessage(rep_addr, packet);
        break;

      default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    }
  }

  messageManager.closeSocket();
}

//=======================================================================
void ParticipantEntity::handleIOThread()
{
  char *command;

  while(true){
    std::cin >> command; 

    if (strcmp(command, "sleep") == 0)
      awake = false;
    else if (strcmp(command, "wakeup") == 0)
      awake = true;
    else if (strcmp(command, "exit") == 0)
      active = false;
    else if (strcmp(command, "enter") == 0)
      active = true;  

    fprintf(stderr, "New status: %s/%s\n", active ? "active" : "inactive", awake ? "awake" : "asleep" );
  }
}

//=======================================================================