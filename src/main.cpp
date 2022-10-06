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
char hostname[100];
bool bDefault;

//=======================================================================
int main(int argc, char **argv)
{
  PORT_SERVER = atoi(argv[1]);
  PORT_CLI = atoi(argv[2]);
  id_in = atoi(argv[3]);
  strcpy(hostname, argv[4]);

  if (atoi(argv[1]) == -1 && atoi(argv[2]) == -1)
    PORT_SERVER = PORT_CLI = 4000;    
    
  bDefault = strcmp(argv[4], "default") == 0; 
  
  signal(SIGINT, signal_callback_handler);
  manager.run();
  return 0;
}

//=======================================================================