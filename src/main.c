#include <stdio.h>
#include "messaging/messages.h"

int main(int argc, char** argv){

  int bManager = 0;

  if(argc > 2){
    exit(1);
  }

  if((argc > 1) && (strcmp(argv[1], "manager") == 0)){
      bManager = 1;
  }

  if(bManager)
    sendMessage();
  else
    receiveMessage();

  return 0;
}