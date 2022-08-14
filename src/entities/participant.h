#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include "../messaging/messages.h"

class ParticipantEntity
{
private:
    bool awake;
    uint16_t rec_port;
public:
    void run();
    void handleMessageThread();
};

#endif