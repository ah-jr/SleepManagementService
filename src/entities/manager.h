#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include "types.h"
#include "../messaging/messages.h"

class ManagerEntity
{
private:
    std::vector<PARTICIPANT> participantVec; 
    uint16_t rec_port;
    uint16_t send_port;

public:
    void run();
    void handleReceiveThread();
    void handleDiscoveryThread();
    void handleMonitoringThread();
};

#endif