#include "messages.h"

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
	char buf[25600];

	if (recvfrom(socketFD, buf, 25600, 0, (struct sockaddr *)rep_addr, &clilen) < 0) 
		printf("ERROR: Could not receive message\n");

	int pos = 0;
	packet->type = *((uint16_t*)&(buf[pos]));
	pos += sizeof(uint16_t);
	packet->active = *((bool*)&(buf[pos]));
	pos += sizeof(bool);
	packet->awake = *((bool*)&(buf[pos]));	
	pos += sizeof(bool);
	strcpy(packet->hostname, &buf[pos]);
	pos += sizeof(char) * MSG_STR_LEN;
	strcpy(packet->ip_addr, &buf[pos]);
	pos += sizeof(char) * MSG_STR_LEN;
	packet->processId = *((uint16_t*)&(buf[pos]));
	pos += sizeof(uint16_t);
	strcpy(packet->mac_addr, &buf[pos]);
	pos += sizeof(char) * MSG_STR_LEN;
	packet->nParticipants = *((uint16_t*)&(buf[pos]));		
	pos += sizeof(uint16_t);

	for(int i = 0; i < packet->nParticipants; i++){
		packet->participants[i] =  *((PARTICIPANT*)&(buf[pos]));
		pos += sizeof(PARTICIPANT);
	}
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
void MessageManager::getIpAddress(char * ip_addr){
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    memcpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);
    ioctl(socketFD, SIOCGIFADDR, &ifr);
    strcpy(ip_addr, inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
}

//=======================================================================
