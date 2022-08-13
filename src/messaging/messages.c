#include "messages.h"
#include "../services/discovery.h"
#include <stdio.h>

#define PORT 4000

void receiveMessage()
{
	int socketFD, pos = 0;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	PACKET receivedPacket;
	char buf[256];
		
  if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;     
	 
	if (bind(socketFD, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not bind socket\n");
	
	clilen = sizeof(struct sockaddr_in);

	while (1) {
		if (recvfrom(socketFD, buf, 256, 0, (struct sockaddr *)&cli_addr, &clilen) < 0) 
			printf("ERROR: Could not receive message\n");

		receivedPacket.type = *((uint16_t*)&(buf[pos]));
		pos += 2;
		receivedPacket.seqn = *((uint16_t*)&(buf[pos]));
		pos += 2;
		receivedPacket.length = *((uint16_t*)&(buf[pos]));
		pos += 2;
		strcpy(receivedPacket.payload, &buf[pos]);

		pos = 0;

		// printf("%d\n", receivedPacket.type);
		// printf("%d\n", receivedPacket.seqn);
		// printf("%d\n", receivedPacket.length);
		// printf("%d\n", receivedPacket.timestamp);

		printf("Received a datagram: %s\n", receivedPacket.payload);

		// switch(receivedPacket.type){
		// 	case SLEEP_DISCOVERY_PACKET:
		// 		sendParticipantAck(cli_addr);
		// 		break;
		// }
	}
	
	close(socketFD);
}

void sendBroadcastMessage(PACKET packet)
{
	int socketFD;
	struct sockaddr_in serv_addr;
	char buf[256];
		
  if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	int broadcastEnable = 1;
	setsockopt(socketFD, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); 

	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");
	
	close(socketFD);
}

void sendMessage(PACKET packet, struct sockaddr_in cli_addr)
{
	int socketFD;
	char buf[256];
		
  if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR: Could not open socket\n");

	if (sendto(socketFD, &packet, sizeof(PACKET), 0, (struct sockaddr *) &cli_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR: Could not send message");
	
	close(socketFD);
}