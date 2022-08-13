#include "messages.h"
#include <stdio.h>

void receiveMessage(uint16_t rec_port, uint16_t rep_port)
{
	int socketFD, pos = 0;
	socklen_t clilen;
	struct sockaddr_in rec_addr, rep_addr;
	PACKET receivedPacket;
	char buf[256];
		
  	if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	memset(&rec_addr, 0, sizeof(rec_addr));
	rec_addr.sin_family = AF_INET;
	rec_addr.sin_port = htons(rec_port);
	rec_addr.sin_addr.s_addr = INADDR_ANY;     
	 
	if (bind(socketFD, (struct sockaddr *)&rec_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not bind socket\n");
	
	clilen = sizeof(struct sockaddr_in);

	while (1) {
		if (recvfrom(socketFD, buf, 256, 0, (struct sockaddr *)&rep_addr, &clilen) < 0) 
			printf("ERROR: Could not receive message\n");

		// Set reply port:
		rep_addr.sin_port = htons(rep_port);

		// Read packet
		pos = 0;
		receivedPacket.type = *((uint16_t*)&(buf[pos]));
		pos += 2;
		receivedPacket.seqn = *((uint16_t*)&(buf[pos]));
		pos += 2;
		receivedPacket.length = *((uint16_t*)&(buf[pos]));
		pos += 2;
		strcpy(receivedPacket.payload, &buf[pos]);

		fprintf(stderr, "Received a datagram: %s\n", receivedPacket.payload);

		switch(receivedPacket.type){
			case SLEEP_DISCOVERY_PACKET : replyMessage(rep_addr, receivedPacket.type, "Discovery_ACK"); break;
			case SLEEP_MONITORING_PACKET: replyMessage(rep_addr, receivedPacket.type, "Awaken"); break;
		}
	}
	close(socketFD);
}

void replyMessage(struct sockaddr_in rep_addr, uint16_t type, const char* payload)
{
	PACKET packet;
	packet.type = type;
	packet.seqn = 0;
	strcpy(packet.payload, payload);
	packet.length = 0;

	sendMessage(packet, rep_addr);
}

void sendBroadcastMessage(PACKET packet, uint16_t send_port)
{
	int socketFD;
	struct sockaddr_in send_addr;
		
  	if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	int broadcastEnable = 1;
	setsockopt(socketFD, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

	memset(&send_addr, 0, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(send_port);
	send_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); 

	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &send_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");
	
	close(socketFD);
}

void sendMessage(PACKET packet, struct sockaddr_in send_addr)
{
	int socketFD;
		
  	if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &send_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");

	close(socketFD);
}