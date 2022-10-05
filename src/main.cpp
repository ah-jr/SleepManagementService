#include "utils.h"
#include "entities/manager.h"
//#include "entities/participant.h"

//=======================================================================
ManagerEntity manager;

//=======================================================================
void signal_callback_handler(int signum) {
  manager.terminate();
}
int PORT_SERVER;
int PORT_CLI;
int id_in;
//=======================================================================
int main(int argc, char **argv)
{
  PORT_SERVER = atoi(argv[1]);
  PORT_CLI = atoi(argv[2]);
  id_in = atoi(argv[3]);
  signal(SIGINT, signal_callback_handler);
  manager.run();
  return 0;
}

//=======================================================================