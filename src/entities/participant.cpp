#include "participant.h"
#include <thread>

void ParticipantEntity::run()
{
    // Initialize variables
    awake = true;
    rec_port = PORT_CLI;

    // Start active services
    std::thread receiveMessageThread(&ParticipantEntity::handleMessageThread, this);  

    // Join Threads
    receiveMessageThread.join();  
}

void ParticipantEntity::handleMessageThread()
{
  struct sockaddr_in rep_addr;
  PACKET packet; 
  MessageManager messageManager; 
  messageManager.setSocket(rec_port, false);

  while (true){
    messageManager.receiveMessage(PORT_SERVER, &rep_addr, &packet);
    
    switch (packet.type){
      case SLEEP_DISCOVERY_PACKET:
        messageManager.replyMessage(rep_addr, packet.type, "Discovery_ACK");
        break;
      case SLEEP_MONITORING_PACKET:
        if (awake) messageManager.replyMessage(rep_addr, packet.type, "Awaken");
        break;
      default : fprintf(stderr, "Wrong packet: %d\n", packet.type);
    }
  }

  messageManager.closeSocket();
}
