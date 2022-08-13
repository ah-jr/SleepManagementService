#include <stdio.h>
#include <thread>
#include "messaging/messages.h"
#include "services/discovery.h"


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
    std::thread t1(&sendDiscoveryPacket, PORT_CLI);
    std::thread t2(&receiveMessage, PORT_SERVER, 0);

    t1.join();
    t2.join();
  }    
  else
    receiveMessage(PORT_CLI, PORT_SERVER);

  return 0;
}