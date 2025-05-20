#include <stdio.h>
#include <string.h>
#include "rcv.h"

int main(int argc, char *argv[]){
  //Check if 2 arguments
  if(argc == 2){
    char *fileName = argv[1];
    tally_t *tally = tally_from_file(fileName);
    if (tally == NULL){
      printf("Could not load votes file. Exiting with error code 1\n");
      return 1;
    }
    tally_election(tally);
    tally_free(tally);
  }
  //check for 4 arguments
  else if (argc == 4){
    if (strcmp(argv[1], "-log") == 0){
      LOG_LEVEL = atoi(argv[2]);
    }
    char *fileName = argv[3];
    tally_t *tally = tally_from_file(fileName);
    if (tally == NULL){
      printf("Could not load votes file. Exiting with error code 1\n");
      return 1;
    }
    tally_election(tally);
    tally_free(tally);
  }
  //break if neither
  else {
    return 1;
  }
}