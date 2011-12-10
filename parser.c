#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main() {
  struct List* list;
  
  list = list_create(LIST_INT);
  list_destroy(list);
  
  
  puts("Hallo Welt.\n");
  return 0;
}