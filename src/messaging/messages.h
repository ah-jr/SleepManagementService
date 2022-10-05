#ifndef MESSAGES_H
#define MESSAGES_H

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "../entities/types.h"

//=======================================================================
#define SLEEP_DISCOVERY_PACKET 1
#define SLEEP_MONITORING_PACKET 2
#define WAKEUP_PACKET 3
#define EXIT_PACKET 4
#define ELECTION_PACKET 5
#define ANSWER_PACKET 6
#define REPLICATION_PACKET 7

#define REPLY_SLEEP_DISCOVERY_PACKET 10
#define REPLY_SLEEP_MONITORING_PACKET 20



//#define PORT_CLI 4000
//#define PORT_SERVER 4001

#define MSG_STR_LEN 20
#define MAX_PARTICIPANTS 2

#define MAC_ADDR_PATH "/sys/class/net/eth0/address"

//=======================================================================
typedef struct packet
{
    uint16_t type;     // Package type (DATA | CMD)
    bool active;
    bool awake;      
    char hostname[MSG_STR_LEN];
    char ip_addr[MSG_STR_LEN];
    uint16_t processId;
    char mac_addr[MSG_STR_LEN];
    uint16_t nParticipants;
    PARTICIPANT participants[MAX_PARTICIPANTS];
} PACKET;

//=======================================================================
class MessageManager
{
private:
	int socketFD;
    struct sockaddr_in skt_addr;

public:
	MessageManager() = default;
    void setSocket(uint16_t skt_port, bool bBroadcast);
    void closeSocket();
	void receiveMessage(struct sockaddr_in* rep_addr, PACKET* packet);
	void replyMessage(struct sockaddr_in rep_addr, PACKET packet);
	void sendMessage(PACKET packet);
    void getIpAddress(char * ip_addr);
};

//=======================================================================
#endif
