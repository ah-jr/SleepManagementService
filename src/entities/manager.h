#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <fstream>
#include "types.h"
#include "../messaging/messages.h"
#include "../deps/cpp-text-table-master/TextTable.h"

//=======================================================================
class ManagerEntity
{
private:
    PARTICIPANT status;
    std::mutex status_mutex;
    std::atomic<bool> update;
    std::map<std::string, PARTICIPANT> pMap; 
    std::mutex pMap_mutex;
    uint16_t rec_port;
    uint16_t send_port;
    bool bIsLeader;
    bool bNeedsElection;
    bool bReceivedElectionAnswer;

public:
    void run();
    void handleReceiveThread();
    void handleMessageThread();
    void handleDiscoveryThread();
    void handleMonitoringThread();
    void handleInterfaceThread();
    void handleManagementThread();
    void handleIOThread();
    void handleElectionThread();
    void handleReplicationThread();
    void incrementCounters(int type);
    void repeatMessage(PACKET packet, float interval);
    void sendMessage(PACKET packet);
    void getMacAddress(char* mac_addr);
    void terminate();
};

//=======================================================================
#endif