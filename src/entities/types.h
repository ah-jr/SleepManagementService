#ifndef ENTITIES_H
#define ENTITIES_H

#include <cstring>

//=======================================================================
typedef struct participant{
  char name[20];
  char mac_addr[20];
  char ip_addr[20];
  bool status;
  friend bool operator<(const participant &a, const participant &b) { 
    return std::strcmp(a.name, b.name) < 0; 
  } 
} PARTICIPANT;

//=======================================================================
#endif