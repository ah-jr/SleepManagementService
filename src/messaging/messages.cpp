#include "messages.h"
#include <stdio.h>

//=======================================================================
void MessageManager::setSocket(uint16_t rec_port, bool bBroadCast){
	if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	if (bBroadCast){		
		int broadcastEnable = 1;
		setsockopt(socketFD, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	}
	else{
		struct sockaddr_in rec_addr;
		memset(&rec_addr, 0, sizeof(rec_addr));
		rec_addr.sin_family = AF_INET;
		rec_addr.sin_port = htons(rec_port);
		rec_addr.sin_addr.s_addr = INADDR_ANY;     
		
		if (bind(socketFD, (struct sockaddr *)&rec_addr, sizeof(struct sockaddr)) < 0) 
			printf("ERROR: Could not bind socket\n");
	}
}

//=======================================================================
void MessageManager::closeSocket(){
	close(socketFD);
}

//=======================================================================
void  MessageManager::receiveMessage(uint16_t rep_port, struct sockaddr_in* rep_addr, PACKET* packet)
{
	socklen_t clilen = sizeof(struct sockaddr_in);
	char buf[256];

	if (recvfrom(socketFD, buf, 256, 0, (struct sockaddr *)rep_addr, &clilen) < 0) 
		printf("ERROR: Could not receive message\n");

	rep_addr->sin_port = htons(rep_port);

	int pos = 0;
	packet->type = *((uint16_t*)&(buf[pos]));
	pos += 2;
	packet->seqn = *((uint16_t*)&(buf[pos]));
	pos += 2;
	packet->length = *((uint16_t*)&(buf[pos]));
	pos += 2;
	strcpy(packet->payload, &buf[pos]);
	fprintf(stderr, "Received a datagram: %s\n", packet->payload);
}

//=======================================================================
void  MessageManager::replyMessage(struct sockaddr_in rep_addr, uint16_t type, const char* payload)
{
	PACKET packet;
	packet.type = type;
	packet.seqn = 0;
	strcpy(packet.payload, payload);
	packet.length = 0;

	sendMessage(packet, rep_addr);
}

//=======================================================================
void  MessageManager::sendBroadcastMessage(PACKET packet, uint16_t send_port)
{
	struct sockaddr_in send_addr;

	memset(&send_addr, 0, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(send_port);
	send_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); 

	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &send_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");
}

//=======================================================================
void MessageManager::sendMessage(PACKET packet, struct sockaddr_in send_addr)
{
	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &send_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");
}

//=======================================================================