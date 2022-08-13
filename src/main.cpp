#include <stdio.h>
#include <thread>
#include "messaging/messages.h"
#include "services/services.h"


void runManager()
{
    // Start active services
    std::thread sendDiscoveryThread(&sendDiscoveryPacket, PORT_CLI);
    std::thread sendMonitoringThread(&sendMonitoringPacket, PORT_CLI);

    // Start passive services
    std::thread receiveMessagesThread(&receiveMessage, PORT_SERVER, 0);

    // Join Threads
    sendDiscoveryThread.join();
    receiveMessagesThread.join();
}

void runParticipant()
{
    bool awake = true;
    
    receiveMessage(PORT_CLI, PORT_SERVER);
}

int main(int argc, char** argv){

  bool bManager = 0;

  if(argc > 2){
    printf("ERROR: Expected 2 arguments but received %d\n", argc);
    exit(1);
  }

  if((argc == 2)){
    if(strcmp(argv[1], "manager") == 0){
      bManager = true;
    }
    else{
      printf("ERROR: %s is not a valid argument", argv[1]);
      exit(1);
    }
  }

  if(bManager){
    runManager();
  }    
  else
    runParticipant();

  return 0;
}
