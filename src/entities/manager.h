#ifndef MANAGER_H
#define MANAGER_H

#include <set>
#include <algorithm>
#include "types.h"
#include "../messaging/messages.h"

//=======================================================================
class ManagerEntity
{
private:
    std::set<PARTICIPANT> pSet; 
    uint16_t rec_port;
    uint16_t send_port;

public:
    void run();
    void handleReceiveThread();
    void handleDiscoveryThread();
    void handleMonitoringThread();
    void handleInterfaceThread();
};

//=======================================================================
#endif