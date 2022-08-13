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

#define SLEEP_DISCOVERY_PACKET 1
#define SLEEP_MONITORING_PACKET 2

#define PORT_CLI 4000
#define PORT_SERVER 4001

typedef struct packet
{
    uint16_t type;     // Tipo do pacote (p.ex. DATA | CMD)
    uint16_t seqn;     // Número de sequência
    uint16_t length;   // Comprimento do payload
    char payload[256]; // Dados da mensagem
} PACKET;

void sendBroadcastMessage(PACKET packet, uint16_t send_port);
void sendMessage(PACKET packet, struct sockaddr_in send_addr);

void receiveMessage(uint16_t rec_port, uint16_t rep_port);
void replyMessage(struct sockaddr_in rep_addr, uint16_t type, const char *payload);

#endif