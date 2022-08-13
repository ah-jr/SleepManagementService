#include "messages.h"
#include <stdio.h>

#define PORT 4000

void receiveMessage()
{
	int socketFD, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buf[256];
		
  if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;     
	 
	if (bind(socketFD, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	clilen = sizeof(struct sockaddr_in);

	while (1) {
		/* receive from socket */
		n = recvfrom(socketFD, buf, 256, 0, (struct sockaddr *) &cli_addr, &clilen);
		if (n < 0) 
			printf("ERROR on recvfrom");
		printf("Received a datagram: %s\n", buf);
	}
	
	close(socketFD);
}

void sendMessage()
{
	int socketFD;
	struct sockaddr_in serv_addr;
	char buf[256];
		
  if ((socketFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	int broadcastEnable = 1;
	setsockopt(socketFD, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); 
	 
	while (1) {		
		/* send to socket */
		if (sendto(socketFD, "Got your message\n", 17, 0, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
			printf("ERROR on sendto");
		
		sleep(3);
	}
	
	close(socketFD);
}