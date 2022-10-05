#ifndef ENTITIES_H
#define ENTITIES_H

#include <cstring>
//#include "../messaging/messages.h"

#define MAX_DISCOVERY_COUNT 15
#define MSG_STR_LEN 20
#define MAX_MONITORING_COUNT 10

//=======================================================================
typedef struct participant{
  char name[MSG_STR_LEN];
  char mac_addr[MSG_STR_LEN];
  char ip_addr[MSG_STR_LEN];
  int discovery_count;
  int monitoring_count;
  bool awake;
  bool active;
  uint16_t id;
  friend bool operator<(const participant &a, const participant &b) { 
    return std::strcmp(a.name, b.name) < 0; 
  } 
} PARTICIPANT;

//=======================================================================
#endif
