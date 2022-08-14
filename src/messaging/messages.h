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

//=======================================================================
#define SLEEP_DISCOVERY_PACKET 1
#define SLEEP_MONITORING_PACKET 2

#define PORT_CLI 4000
#define PORT_SERVER 4001

//=======================================================================
typedef struct packet
{
    uint16_t type;     // Package type (DATA | CMD)
    uint16_t seqn;     // Seq number
    uint16_t length;   // Payload's length
    char payload[256]; // Message data
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
};

//=======================================================================
#endif