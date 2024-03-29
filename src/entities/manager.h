#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include "types.h"
#include "../messaging/messages.h"
#include "../deps/cpp-text-table-master/TextTable.h"

//=======================================================================
class ManagerEntity
{
private:
    std::atomic<bool> update;
    std::map<std::string, PARTICIPANT> pMap; 
    std::mutex pMap_mutex;
    uint16_t rec_port;
    uint16_t send_port;

public:
    void run();
    void handleReceiveThread();
    void handleDiscoveryThread();
    void handleMonitoringThread();
    void handleInterfaceThread();
    void handleManagementThread();
    void handleIOThread();
    void incrementCounters(int type);
    void repeatMessage(PACKET packet, float interval);
    void sendMessage(PACKET packet);
};

//=======================================================================
#endif