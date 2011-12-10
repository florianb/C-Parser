#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct List* list_create(int type) {
  struct List* newList;
  int elementSize = 0;
  
  if ((LIST_STRING <= type) && (type < LIST_USER_DEFINED))
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
      case LIST_STRING:
        elementSize = -1;
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
  newList->compare = LIST_UNDEFINED;
  
  return newList;
}

int list_destroy(struct List* list) {
  free(list);
  return 0;
}

int list_length(struct List* list) {
  return list->length;
}

int list_type() {
  return 0;  
}

void* list_elementInternal() {
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

int list_compareElements() {
  return 0;  
}

void list_prettyPrint(struct List* list) {
  char stringCache[LIST_STRING_CACHE_SIZE] = "";
  
  printf("List (Header-Position: %X)\n\n", (int) list);
  
  switch (list->type)
  {
    case LIST_BOOL:
      strncpy(stringCache, "_Bool", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_CHAR:
      strncpy(stringCache, "char", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_SIGNED_CHAR:
      strncpy(stringCache, "signed char", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_UNSIGNED_CHAR:
      strncpy(stringCache, "unsigned char", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_SHORT:
      strncpy(stringCache, "short", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_UNSIGNED_SHORT:
      strncpy(stringCache, "unsigned short", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_INT:
      strncpy(stringCache, "int", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_UNSIGNED_INT:
      strncpy(stringCache, "unsigned int", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_LONG:
      strncpy(stringCache, "long", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_UNSIGNED_LONG:
      strncpy(stringCache, "unsigned long", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_LONG_LONG:
      strncpy(stringCache, "long long", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_UNSIGNED_LONG_LONG:
      strncpy(stringCache, "unsigned long long", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_FLOAT:
      strncpy(stringCache, "float", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_DOUBLE:
      strncpy(stringCache, "double", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_LONG_DOUBLE:
      strncpy(stringCache, "long double", LIST_STRING_CACHE_SIZE);
      break;
    case LIST_STRING:
      strncpy(stringCache, "string", LIST_STRING_CACHE_SIZE);
      break;
  }
  printf("  - Contains elements of the type \"%s\" (%d bytes per element).\n",
          stringCache,
          list->elementSize);
  printf("  - Contains %d element/s.\n", list->length);
  if (list->type != LIST_STRING)
  {
    printf("  - Complete size: %d bytes\n\n", (int) (sizeof(struct List) +
            sizeof(struct ListElement) * list->length +
            list->elementSize * list->length));
  }
  else
  {
    puts("  - The complete size can't be calculated for strings.\n\n");
  }

  if (list->toString == LIST_UNDEFINED)
    strncpy(stringCache, "not", LIST_STRING_CACHE_SIZE);
  else
    strncpy(stringCache, "", LIST_STRING_CACHE_SIZE);
  printf("  - A toString()-callback is %s defined.\n", stringCache);

  if (list->compare == LIST_UNDEFINED)
    strncpy(stringCache, "not", LIST_STRING_CACHE_SIZE);
  else
    strncpy(stringCache, "", LIST_STRING_CACHE_SIZE);
  printf("  - A compare()-callback is %s defined.\n", stringCache);
  
  puts("\n");
}

void list_prettyPrintElement() {
  
}