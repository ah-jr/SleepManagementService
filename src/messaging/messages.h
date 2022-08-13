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
#define SLEEP_DISCOVERY_PACKET_ACK 2

typedef struct packet{
 uint16_t type; //Tipo do pacote (p.ex. DATA | CMD)
 uint16_t seqn; //Número de sequência
 uint16_t length; //Comprimento do payload 
 char payload[256]; //Dados da mensagem
}PACKET;

void sendBroadcastMessage(PACKET packet);
void sendMessage(PACKET packet, struct sockaddr_in cli_addr);
void receiveMessage();

#endif