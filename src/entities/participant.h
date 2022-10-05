#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <iostream>
#include <thread>
#include <fstream>
#include <mutex>
#include "types.h"
#include "../messaging/messages.h"
#include "../deps/cpp-text-table-master/TextTable.h"

//=======================================================================
class ParticipantEntity
{
private:
    PARTICIPANT status;
    std::mutex status_mutex;
    uint16_t rec_port;
public:
    void run();
    void terminate();
    void handleMessageThread();
    void handleInterfaceThread();
    void handleIOThread();
    void getMacAddress(char* mac_addr);
};

//=======================================================================
#endif