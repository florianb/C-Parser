#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main() {
  puts("\nparser 0.01 - 2011 by florianbreisch@b-lebt.de\n\n");
  
  int a = 5;
  int b = 9;
  
  struct List* list;
  
  list = list_create(LIST_INT);
  
  list_prettyPrint(list);
  
  list_insertBefore(list, 0, &a);
  
  list_prettyPrint(list);
  //list_setContent(list, 0, &b);
  
  list_insertBefore(list, 0, &b);
  
  list_element(list, 0);
  
  list_prettyPrint(list);
  
  list_destroy(list);
  
  
  puts("Hallo Welt.\n");
  return 0;
}