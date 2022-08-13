#include <stdio.h>
#include "messaging/messages.h"
#include "services/discovery.h"
#include "utils.h"

int main(int argc, char** argv){

  bool bManager = 0;

  if(argc > 2){
    printf("ERROR: Expected 2 arguments but received %d\n", argc);
    exit(1);
  }

  if((argc == 2)){
    if(strcmp(argv[1], "manager") == 0){
      bManager = TRUE;
    }
    else{
      printf("ERROR: %s is not a valid argument", argv[1]);
      exit(1);
    }
  }

  if(bManager)
    sendDiscoveryPacket(TRUE);
  else
    receiveMessage();

  return 0;
}