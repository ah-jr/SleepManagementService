#include <stdio.h>
#include <thread>
#include <vector>
#include "messaging/messages.h"
#include "services/services.h"

typedef struct participant{
  char name[10];
  char* mac_addr;
  char* ip_addr;
  bool status;
} PARTICIPANT;

void handleReceive(uint16_t rec_port, std::vector<PARTICIPANT>& participantsVec){
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

void runManager()
{
  std::vector<PARTICIPANT> participantsVec; 

  // Start active services
  std::thread sendDiscoveryThread(&handleDiscoveryPacket, PORT_CLI);
  std::thread sendMonitoringThread(&handleMonitoringPacket, PORT_CLI);
  //std::thread managementThread(&handleManagement); 

  // Start passive services
  std::thread receiveMessagesThread(&handleReceive, PORT_SERVER, std::ref(participantsVec));

  // Join Threads
  sendDiscoveryThread.join();
  sendMonitoringThread.join();
  receiveMessagesThread.join();
}

void runParticipant()
{
  bool awake = true;
  struct sockaddr_in rep_addr;
  PACKET packet;
  MessageManager messageManager; 
  messageManager.setSocket(PORT_CLI, false);

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

int main(int argc, char **argv)
{
  bool bManager = 0;

  if (argc > 2){
    printf("ERROR: Expected 2 arguments but received %d\n", argc);
    exit(1);
  }

  if ((argc == 2)){
    if (strcmp(argv[1], "manager") == 0){
      bManager = true;
    }
    else{
      printf("ERROR: %s is not a valid argument", argv[1]);
      exit(1);
    }
  }

  if (bManager)
    runManager();
  else
    runParticipant();

  return 0;
}
