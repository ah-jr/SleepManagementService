#include "utils.h"
#include "entities/manager.h"
#include "entities/participant.h"

//=======================================================================
ParticipantEntity participant;
ManagerEntity manager;

//=======================================================================
void signal_callback_handler(int signum) {
  participant.terminate();
}

//=======================================================================
int main(int argc, char **argv)
{
  if (checkInput(argc, argv)){
    manager.run();
  }
  else{
    signal(SIGINT, signal_callback_handler);
    participant.run();
  }

  return 0;
}

//=======================================================================