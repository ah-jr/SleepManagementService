#include "utils.h"
#include "entities/manager.h"
#include "entities/participant.h"

//=======================================================================
ManagerEntity manager;

//=======================================================================
void signal_callback_handler(int signum) {
  manager.terminate();
}

//=======================================================================
int main(int argc, char **argv)
{
  signal(SIGINT, signal_callback_handler);
  manager.run();
  return 0;
}

//=======================================================================