#include <stdio.h>
#include <stdlib.h>
#include <cstring>

//=======================================================================
bool checkInput(int argc, char **argv){
  if (!(argc == 1 or argc == 2)){
    printf("ERROR: Expected 0 or 1 argument but received %d\n", argc - 1);
    exit(1);
  }

  if (argc == 2){
    if (strcmp(argv[1], "manager") == 0)
      return true;
    else{
      printf("ERROR: %s is not a valid argument\n", argv[1]);
      exit(1);
    }
  }

  return false;
}

//=======================================================================