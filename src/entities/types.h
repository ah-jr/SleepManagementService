#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct participant{
  char name[10];
  char* mac_addr;
  char* ip_addr;
  bool status;
} PARTICIPANT;

#endif