#include <stdio.h>
#include <thread>
#include "messaging/messages.h"
#include "services/services.h"


void handleReceive(uint16_t rec_port){
  struct sockaddr_in rep_addr;
  PACKET packet;
  MessageManager messageManager; 
  messageManager.setSocket(rec_port);

  while (1){
    memset(&packet, 0, sizeof(packet));
    memset(&rep_addr, 0, sizeof(rep_addr));
    messageManager.receiveMessage(0, &rep_addr, &packet);
  }

  messageManager.closeSocket();
}

void runManager()
{
  // Start active services
  MessageManager messageManager; 
  messageManager.setSocket(PORT_CLI);

  std::thread sendDiscoveryThread(&handleDiscoveryPacket, PORT_CLI, messageManager);
  std::thread sendMonitoringThread(&handleMonitoringPacket, PORT_CLI, messageManager);

  messageManager.closeSocket();

  // Start passive services
  std::thread receiveMessagesThread(&handleReceive, PORT_SERVER);

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
  messageManager.setSocket(PORT_CLI);

  while (1){
    memset(&packet, 0, sizeof(packet));
    memset(&rep_addr, 0, sizeof(rep_addr));

    messageManager.receiveMessage(PORT_SERVER, &rep_addr, &packet);

    switch (packet.type)
    {
    case SLEEP_DISCOVERY_PACKET:
      messageManager.replyMessage(rep_addr, packet.type, "Discovery_ACK");
      break;
    case SLEEP_MONITORING_PACKET:
      messageManager.replyMessage(rep_addr, packet.type, "Awaken");
      break;
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

  if (bManager){
    runManager();
  }
  else
    runParticipant();

  return 0;
}
