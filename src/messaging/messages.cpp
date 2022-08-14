#include "messages.h"
#include <stdio.h>

//=======================================================================
void MessageManager::setSocket(uint16_t skt_port, bool bBroadcast){
	memset(&skt_addr, 0, sizeof(skt_addr));
	skt_addr.sin_family = AF_INET;
	skt_addr.sin_port = htons(skt_port);

	if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	if (bBroadcast){		
		skt_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); 
		int broadcastEnable = 1;
		setsockopt(socketFD, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	}
	else{
		skt_addr.sin_addr.s_addr = INADDR_ANY;     
		
		if (bind(socketFD, (struct sockaddr *)&skt_addr, sizeof(struct sockaddr)) < 0) 
			printf("ERROR: Could not bind socket\n");
	}
}

//=======================================================================
void MessageManager::closeSocket(){
	close(socketFD);
}

//=======================================================================
void  MessageManager::receiveMessage(struct sockaddr_in* rep_addr, PACKET* packet)
{
	socklen_t clilen = sizeof(struct sockaddr_in);
	char buf[256];

	if (recvfrom(socketFD, buf, 256, 0, (struct sockaddr *)rep_addr, &clilen) < 0) 
		printf("ERROR: Could not receive message\n");

	int pos = 0;
	packet->type = *((uint16_t*)&(buf[pos]));
	pos += 2;
	packet->seqn = *((uint16_t*)&(buf[pos]));
	pos += 2;
	packet->length = *((uint16_t*)&(buf[pos]));
	pos += 2;
	strcpy(packet->payload, &buf[pos]);
	//fprintf(stderr, "Received a datagram: %s\n", packet->payload);
}

//=======================================================================
void MessageManager::replyMessage(struct sockaddr_in rep_addr, PACKET packet)
{
	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &rep_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");
}

//=======================================================================
void MessageManager::sendMessage(PACKET packet)
{
	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &skt_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");
}

//=======================================================================