#include <stdio.h>
#include "treemap.h"
int main(){
  if((sizeof(treemap_t) != 8) || (sizeof(tmnode_t) != 208)){
    printf("Anti-theft countermeasures engaged\n");
    return 1;
  }
  return 0;
}
