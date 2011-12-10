#include "list.h"

#include <stdlib.h>

struct List* list_create(int type) {
  struct List* newList;
  int elementSize = 0;
  
  if (LIST_LONG_DOUBLE <= type < LIST_USER_DEFINED)
  {
    switch (type)
    {
      case LIST_BOOL:
        elementSize = sizeof(_Bool);
        break;
      case LIST_CHAR:
        elementSize = sizeof(char);
        break;
      case LIST_SIGNED_CHAR:
        elementSize = sizeof(signed char);
        break;
      case LIST_UNSIGNED_CHAR:
        elementSize = sizeof(unsigned char);
        break;
      case LIST_SHORT:
        elementSize = sizeof(short);
        break;
      case LIST_UNSIGNED_SHORT:
        elementSize = sizeof(unsigned short);
        break;
      case LIST_INT:
        elementSize = sizeof(int);
        break;
      case LIST_UNSIGNED_INT:
        elementSize = sizeof(unsigned int);
        break;
      case LIST_LONG:
        elementSize = sizeof(long);
        break;
      case LIST_UNSIGNED_LONG:
        elementSize = sizeof(unsigned long);
        break;
      case LIST_LONG_LONG:
        elementSize = sizeof(long long);
        break;
      case LIST_UNSIGNED_LONG_LONG:
        elementSize = sizeof(unsigned long long);
        break;
      case LIST_FLOAT:
        elementSize = sizeof(float);
        break;
      case LIST_DOUBLE:
        elementSize = sizeof(double);
        break;
      case LIST_LONG_DOUBLE:
        elementSize = sizeof(long double);
        break;
    }
  }
  else if (type > LIST_USER_DEFINED)
  {
    elementSize = type;
    type = 0;
  }
  else
  {
    return LIST_UNDEFINED;
  }
  
  newList = malloc(sizeof(struct List));
  
  newList->elementSize = elementSize;
  newList->length = 0;
  newList->type = type;
  newList->toString = LIST_UNDEFINED;
  newList->equals = LIST_UNDEFINED;
  
  return newList;
}

int list_destroy(struct List* list) {
  free(list);
  return 0;
}

int list_length() {
  return 0;  
}

int list_type() {
  return 0;  
}

void* list_element() {
  return 0;  
}

int list_insert() {
  return 0;
}

int list_append() {
  return 0;
}

int list_push() {
  return 0;  
}

int list_pop() {
  return 0;  
}

int list_remove() {
  return 0;  
}

int list_toString() {
  return 0;
}

int list_sort() {
  return 0;  
}

int list_equals() {
  return 0;  
}

void list_prettyPrintList() {
  
}

void list_prettyPrintElement() {
  
}