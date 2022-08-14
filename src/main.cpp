#include "utils.h"
#include "entities/manager.h"
#include "entities/participant.h"

int main(int argc, char **argv)
{
  if (checkInput(argc, argv)){
    ManagerEntity manager;
    manager.run();
  }
  else{
    ParticipantEntity participant;
    participant.run();
  }

  return 0;
}
