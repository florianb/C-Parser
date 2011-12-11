#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main() {
  puts("\nparser 0.01 - 2011 by florianbreisch@b-lebt.de\n\n");
  
  int a = 5;
  int b = 9;
  
  char c[] = "Test!";
  char d[] = "Und noch ein Test!!";
  char e[] = "Test!";
  char f[] = "Und noch ein Test!!";
  
  struct List* list;
  
  list = list_create(LIST_STRING);
  
  list_prettyPrint(list);
  
  //list_insertBefore(list, 0, &a);
  
  list_insertAfter(list, -1, "Added 1st.");
  list_insertAfter(list, -1, "Added 2nd.");
  list_insertAfter(list, -1, "Added 3rd.");
  list_insertAfter(list, -1, "Added 4th.");
  list_prettyPrint(list);
  //list_setContent(list, 0, &b);
  
  //list_insertBefore(list, 0, &b);
  
  //printf("Value of &a: %d\n", *((int*) list_element(list, 1)));
  //printf("Value of &b: %d\n", *((int*) list_element(list, 0)));
  
  //printf("Value of &c: %s\n", list_element(list, 1));
  //printf("Value of &d: %s\n", list_element(list, 0));
  
  list_prettyPrint(list);
  
  list_destroy(list);
  
  
  puts("Hallo Welt.\n");
  return 0;
}