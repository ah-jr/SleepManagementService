#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <iostream>
#include <thread>
#include <fstream>
#include "../messaging/messages.h"

//=======================================================================
class ParticipantEntity
{
private:
    bool active;
    bool awake;
    uint16_t rec_port;
public:
    void run();
    void handleMessageThread();
    void handleIOThread();
    void getMacAddress(char* mac_addr);
};

//=======================================================================
#endif