#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "amachine.h"

int main() {
  puts("\nparser 0.01 - 2011 by florianbreisch@b-lebt.de\n\n");
  
  int a = 5;
  int b = 9;
  int c = 8;
  int d = 7;
  int e = 1;
  int f = -10;
  int g = 13;
  int h = 100;
  
  /*
  char c[] = "Test!";
  char d[] = "Und noch ein Test!!";
  char e[] = "Test!";
  char f[] = "Und noch ein Test!!";
  */
  
  struct List* list;
  
  //list = list_create(LIST_STRING);
  list = list_create(LIST_INT);
  
  //list_prettyPrint(list);
  
  //list_insertBefore(list, 0, &a);
  
  //list_insertAfter(list, 0, "Added 1st.");
  //list_insertAfter(list, 0, "Added 2nd.");
  //list_insertAfter(list, 0, "Added 3rd.");
  //list_insertAfter(list, 0, "Added 4th.");
  list_insertAfter(list, 0, &a);
  list_insertAfter(list, 0, &b);
  list_insertAfter(list, 0, &c);
  list_insertAfter(list, 0, &d);
  list_insertAfter(list, 0, &e);
  list_insertAfter(list, 0, &f);
  list_insertAfter(list, 0, &g);
  list_insertAfter(list, 0, &h);
  
  //list_prettyPrint(list);
  list_sort(list);
  //list_prettyPrint(list);

  //list_setContent(list, 1, "Neuer Inhalt.");
  //list_remove(list, 0);
  //list_prettyPrint(list);
  //list_setContent(list, 0, &b);
  
  //list_insertBefore(list, 0, &b);
  
  //printf("Value of &a: %d\n", *((int*) list_element(list, 1)));
  //printf("Value of &b: %d\n", *((int*) list_element(list, 0)));
  
  //printf("Value of &c: %s\n", list_element(list, 1));
  //printf("Value of &d: %s\n", list_element(list, 0));
  

  
  list_destroy(list);
    
  FILE* file = fopen("test/Test.c", "r");
  amch_run(&list, &file);
  list_prettyPrint(list);
  list_destroy(list);
  return 0;
}