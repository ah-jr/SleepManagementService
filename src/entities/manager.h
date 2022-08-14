#ifndef MANAGER_H
#define MANAGER_H

#include <set>
#include <algorithm>
#include <thread>
#include <mutex>
#include "types.h"
#include "../messaging/messages.h"
#include "../deps/cpp-text-table-master/TextTable.h"

//=======================================================================
class ManagerEntity
{
private:
    std::set<PARTICIPANT> pSet; 
    std::mutex pSet_mutex;
    uint16_t rec_port;
    uint16_t send_port;

public:
    void run();
    void handleReceiveThread();
    void handleDiscoveryThread();
    void handleMonitoringThread();
    void handleInterfaceThread();
    void broadcastMessage(PACKET packet);
};

//=======================================================================
#endif