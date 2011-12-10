#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void list_prettyPrintElement(struct ListElement* given_element) {
  printf("  # Element %X\n", (unsigned int) given_element);
  printf("     - next:     %X\n", (unsigned int) given_element->nextElement);
  printf("     - previous: %X\n", (unsigned int) given_element->previousElement);
  printf("     - content:  %X\n\n", (unsigned int) given_element->content);
}

/**
  Erstellt eine leere Liste vom angegebenen Typ
*/
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

/**
  Gibt einen Zeiger auf das ListElement am angegebenen Index zurück
*/
struct ListElement* list_elementInternal(struct List* list, int index) {
  if (list->length == 0)
  {
    return (struct ListElement*) LIST_UNDEFINED;
  }
  struct ListElement* current_element;
  int current_index = 0;
  current_element = list->firstElement;

  printf("First-Element while searching for #%d:\n", index);
  list_prettyPrintElement(current_element);

  while (current_index != index)
  {
    if (current_index > index)
    {
      current_index--;
      current_element = current_element->previousElement;
    }
    else
    {
      current_index++;
      current_element = current_element->nextElement;
    }
    
    puts("Destroying:\n");
    list_prettyPrintElement(current_element);
  }
  return current_element;  
}

/**
  Gibt einen Zeiger auf den Content des Elements am angegebenen Index zurück
*/
void* list_element(struct List* list, int index) {
  struct ListElement* current_element = list_elementInternal(list, index)->content;
  if (current_element == LIST_UNDEFINED)
  {
    return (void*) current_element;
  }
  else
  {
    return current_element->content;
  }
}

/**
  Erstellt ein leeres, auf sich selbst zeigendes Listenelement
*/
struct ListElement* list_createElement()
{
  struct ListElement* new_element = malloc(sizeof(struct ListElement));
  new_element->previousElement = new_element;
  new_element->nextElement = new_element;
  new_element->content = (struct ListElement*) LIST_UNDEFINED;
  
  puts("Created:\n");
  list_prettyPrintElement(new_element);
  
  return new_element;
}

/**
  Gibt das Element mit dem enthaltenen Inhalt frei
*/
int list_destroyElement(struct ListElement* element)
{
  puts("Destroying:\n");
  list_prettyPrintElement(element);
  
  if (element->content != LIST_UNDEFINED)
  {
    puts("destroyed element-content.\n");
    free(element->content);
  }
  free(element);
  return LIST_SUCCESS;
}

/**
  Gibt die komplette Liste mit allen enthaltenen Elementen frei
*/
int list_destroy(struct List* list) {
  if (list->length > 0)
  {
    struct ListElement* next_element = list->firstElement;
    next_element = next_element->nextElement;
    list_destroyElement(list->firstElement);
    puts("freed once.\n");
    while (list->firstElement != next_element)
    {
      puts("freed another while..\n");
      next_element = next_element->nextElement;
      list_destroyElement(next_element->previousElement);
    }
  }
  free(list);
  return LIST_SUCCESS;
}

/**
  Setzt den Inhalt eines Elements mit dem angegebenen Content, der Typ wird durch die Liste erkannt
*/
int list_setContent(struct List* list, int index, void* new_content) {
  if (list->length == 0)
  {
    return LIST_FAILURE;
  }
  
  void* old_content = list_element(list, index);
  
  if (list->type == LIST_STRING) {
    if (old_content != LIST_UNDEFINED)
    {
      free(old_content);
    }
    old_content = malloc(strlen(new_content) + 1);
    strcpy(old_content, new_content);
  }
  else
  {
    if (old_content == LIST_UNDEFINED)
    {
      old_content = malloc(list->elementSize);
    }
    memcpy(old_content, new_content, list->elementSize);
  }
  
  return LIST_SUCCESS;
}

/**
  Fügt ein neues Element mit dem angegebenen Inhalt vor dem Element an dem gegebenen Index ein, das bestehende Element wir dnach hinten verschoben
*/
int list_insertBefore(struct List* list, int index, void* content) {
  struct ListElement* new_element = list_createElement();
  if (list->length == 0) {
    list->firstElement = new_element;
  }
  else
  {
    struct ListElement* given_element = list_elementInternal(list, index);
    if (list->firstElement == given_element)
    {
      list->firstElement = new_element;
    }
    given_element->previousElement->nextElement = new_element;
    given_element->previousElement = new_element;
  }
  list->length++;
  return list_setContent(list, index, content);
}

/**
  Fügt ein neues Element mit dem angegebenen Inhalt hinter dem Element am angegebenen Index ein
*/
int list_insertAfter(struct List* list, int index, void* content) {
  struct ListElement* new_element = list_createElement();
  if (list->length == 0) {
    list->firstElement = new_element;
  }
  else
  {
    struct ListElement* given_element = list_elementInternal(list, index);
    given_element->nextElement->previousElement = new_element;
    given_element->nextElement = new_element;
  }
  list->length++;
  return list_setContent(list, index + 1, content);
}

/**
  Gibt die Anzahl der enthaltenen Elemente zurück
*/
int list_length(struct List* list) {
  return list->length;
}

/**
  Gibt die Größe eine Elements zurück
*/
int list_type(struct List* list) {
  return list->type;
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
  
  printf("# First Element at: %X\n", (unsigned int) list->firstElement);
  
  puts("\n");
}
