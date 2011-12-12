#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
    type = LIST_USER_DEFINED;
  }
  else
  {
    return LIST_UNDEFINED;
  }
  
  newList = malloc(sizeof(struct List));
  if (newList == NULL) {
    puts("list_create:1: Fehler, Speicher konnte nicht reserviert werden.");
    exit(1);
  }
  
  newList->elementSize = elementSize;
  newList->length = 0;
  newList->type = type;
  newList->toString = NULL;
  newList->compare = NULL;
  newList->setContent = NULL;
  newList->destroyContent = NULL;
  newList->firstElement = NULL;
  
  printf("\nCreated list %p, containing %d element/s..\n", newList, newList->length);
  return newList;
}


int helper_modulo(int a, int b)
{
  int result = a;
  if (a < 0)
  {
    result *= -1;
  }
  result = result % b;
  if (a < 0)
  {
    result *= -1;
  }
  return result;
}

/**
  Gibt einen Zeiger auf das ListElement am angegebenen Index zurück
*/
struct ListElement* list_elementInternal(struct List* list, int index) {
  index = helper_modulo(index, list->length);
  
  if (list->length == 0)
  {
    return (struct ListElement*) LIST_UNDEFINED;
  }
  struct ListElement* current_element;
  int current_index = 0;
  current_element = list->firstElement;

  //printf("First-Element while searching for #%d:\n", index);
  //list_prettyPrintElement(current_element);

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
    
    //puts("Got next element:");
    //list_prettyPrintElement(current_element);
  }
  return current_element;  
}

/**
  Gibt einen Zeiger auf den Content des Elements am angegebenen Index zurück
*/
void* list_element(struct List* list, int index) {
  //printf("Asking for element #%d.\n", index);
  struct ListElement* current_element = list_elementInternal(list, index);
  //puts("Element found:");
  //list_prettyPrintElement(current_element);
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
  if (new_element == NULL) {
    puts("list_createElement:1: Fehler, Speicher konnte nicht reserviert werden.");
    exit(1);
  }
  new_element->previousElement = new_element;
  new_element->nextElement = new_element;
  new_element->content = (struct ListElement*) LIST_UNDEFINED;
  
  //puts("Created:");
  //list_prettyPrintElement(new_element);
  
  return new_element;
}

/**
  Gibt das Element mit dem enthaltenen Inhalt frei
*/
int list_destroyElement(struct List* list, struct ListElement* element)
{
  if (element->content != LIST_UNDEFINED)
  {
    if (list->destroyContent == LIST_UNDEFINED)
    {
      puts("Freeing standard-content..");
      free(element->content);
    }
    else
    {
      puts("Freeing by custom destroyContent().");
      void (*customDestroyContent) (void*);
      customDestroyContent = list->destroyContent;
      (*customDestroyContent) (element);
    }
  }
  printf("Freeing element %p finally..\n", element);
  free(element);
  return LIST_SUCCESS;
}

/**
  Gibt die komplette Liste mit allen enthaltenen Elementen frei
*/
int list_destroy(struct List* list) {
  printf("\nFreeing list %p, containing %d element/s..\n", list, list->length);
  if (list->length > 0)
  {
    struct ListElement* element;
    struct ListElement* next_element;
    next_element = list->firstElement;
    for (int i = 0; i < list->length; i++)
    {
      element = next_element;
      next_element = next_element->nextElement;
      printf("The list must free the element %p.\n", element);
      list_destroyElement(list, element);
    }
  }
  printf("Freeing list %p finally..\n", list);
  free(list);
  return LIST_SUCCESS;
}

/**
  Setzt den Inhalt eines Elements mit dem angegebenen Content, der Typ wird durch die Liste erkannt
*/
int list_setContent(struct List* list, int index, void* new_content)
{
  
  if (list->length == 0)
  {
    return LIST_FAILURE;
  }
  struct ListElement* element = list_elementInternal(list, index);
  
  if (list->setContent != LIST_UNDEFINED)
  {
    printf("Running custom setContent() for %p.\n", new_content);
    void* (*customSetContent) (void*);
    customSetContent = list->setContent;
    element->content = (*customSetContent) (new_content);
    puts("Custom setContent() finished..");
  }
  else
  {
    

    //printf("Overwriting content probably at %p\n", element->content);

    int size = list->elementSize;
    if (list->type == LIST_STRING)
      size = strlen(new_content) + 1;

    //printf("Preparing to write %d bytes.\n", size); 

    else if (element->content == LIST_UNDEFINED || list->type == LIST_STRING)
    {
      if (list->type == LIST_STRING)
      {
        free(element->content);
        //puts("Freed string-content.");
      }
  
      element->content = malloc(size);
      if (element->content == NULL) {
        puts("list_setContent:2: Fehler, Speicher konnte nicht reserviert werden.");
        exit(2);
      }
      //puts("Allocated memory for content.");
    } 
    //printf("Copying content to %p\n", element->content);
    memcpy(element->content, new_content, size);
  }
  return LIST_SUCCESS;
}

/**
  Fügt ein neues Element mit dem angegebenen Inhalt vor dem Element an dem gegebenen Index ein, das bestehende Element wir dnach hinten verschoben
*/
int list_insertBefore(struct List* list, int index, void* content) {
  struct ListElement* new_element = list_createElement();
  
  index = index % list->length;
  
  //printf("Insert content before #%d.\n", index);
  if (list->length == 0) {
    list->firstElement = new_element;
    //puts("First element of list set.");
  }
  else
  {
    index = helper_modulo(index, list->length);
    struct ListElement* given_element = list_elementInternal(list, index);
    //puts("Modifying Element:");
    //list_prettyPrintElement(given_element);
    if (list->firstElement == given_element)
    {
      list->firstElement = new_element;
      //puts("Reset first element of list.");
    }
    new_element->nextElement = given_element;
    new_element->previousElement = given_element->previousElement;
    given_element->previousElement->nextElement = new_element;
    given_element->previousElement = new_element;
    //puts("Modification finished:");
    //list_prettyPrintElement(given_element);
  }
  list->length++;
  //puts("Increased length of list.");
  if (index < 0) /// Ausgleich der Index-Verschiebung durch Einfügen des neuen Elements
    index--;
  
  return list_setContent(list, index, content);
}

/**
  Fügt ein neues Element mit dem angegebenen Inhalt hinter dem Element am angegebenen Index ein
*/
int list_insertAfter(struct List* list, int index, void* content) {
  struct ListElement* new_element = list_createElement();
  
  printf("Working with raw element #%d (length: %d).\n", index, list->length);
  
  if (list->length == 0) {
    list->firstElement = new_element;
  }
  else
  {
    index = helper_modulo(index, list->length);
    printf("Working with element #%d (length: %d).\n", index, list->length);
    struct ListElement* given_element = list_elementInternal(list, index);
    new_element->nextElement = given_element->nextElement;
    new_element->previousElement = given_element;
    given_element->nextElement->previousElement = new_element;
    given_element->nextElement = new_element;
  }
  list->length++;
  if (index >= 0) /// Ausgleich der Index-Verschiebung durch Einfügen des neuen Elements
    index++;
  
  return list_setContent(list, index, content);
}

/**
  Enfernt das Element mit dem angegebenen Index aus der Liste.
*/
int list_remove(struct List* list, int index) {
  if (list->length > 0)
  {
    struct ListElement* element = list_elementInternal(list, index);
    
    if (list->firstElement == element)
    {
      list->firstElement = element->nextElement;
    }
    if (list->length > 1)
    {
      element->nextElement->previousElement = element->previousElement;
      element->previousElement->nextElement = element->nextElement;
    }
    list_destroyElement(list, element);
    list->length--;
  }
  else
  {
    return LIST_FAILURE;
  }
  return LIST_SUCCESS;
}

/**
  Hängt ein Element, mit content als Inhalt, an die Liste an
*/
int list_append(struct List* list, void* content) {
  return list_insertAfter(list, -1, content);
}

/**
  Hängt ein Element, mit content als Inhalt, an die Liste an (Alias für list_append)
*/
int list_push(struct List* list, void* content) {
  return list_append(list, content);
}

/**
  Kopiert den Inhalt des letzten Elements der Liste in content und entfernt das Element anschließend
  aus der Liste
*/
int list_pop(struct List* list, void* content) {
  if (list->type == LIST_STRING)
  {
    strcpy(list_element(list, -1), content);
  }
  else
  {
    memcpy(list_element(list, -1), content, list->elementSize);
  }
  list_remove(list, -1);
  return LIST_SUCCESS;
}

void list_elementContentToString(struct List* list, struct ListElement* element, char* destination, int max) {
  void* content = element->content;
  
  if (list->type != LIST_USER_DEFINED && list->toString == LIST_UNDEFINED)
  {
    switch (list->type)
    {
      case LIST_BOOL:
        if (*((short*) content) == 0)
          snprintf(destination, max, "false (%d)", *((short*) content));
        else
          snprintf(destination, max, "true (%d)", *((short*) content));
        break;
      case LIST_CHAR:
        snprintf(destination, max, "%c", *((char*) content));
        break;
      case LIST_SIGNED_CHAR:
        snprintf(destination, max, "%c (%hhd)", *((char*) content), *((signed char*) content));
        break;
      case LIST_UNSIGNED_CHAR:
        snprintf(destination, max, "%c", *((char*) content));
        break;
      case LIST_SHORT:
        snprintf(destination, max, "%hd", *((short*) content));
        break;
      case LIST_UNSIGNED_SHORT:
        snprintf(destination, max, "%hd", *((unsigned short*) content));
        break;
      case LIST_INT:
        snprintf(destination, max, "%d", *((int*) content));
        break;
      case LIST_UNSIGNED_INT:
        snprintf(destination, max, "%d", *((unsigned short*) content));
        break;
      case LIST_LONG:
        snprintf(destination, max, "%ld", *((long*) content));
        break;
      case LIST_UNSIGNED_LONG:
        snprintf(destination, max, "%ld", *((unsigned long*) content));
        break;
      case LIST_LONG_LONG:
        snprintf(destination, max, "%lld", *((long long*) content));
        break;
      case LIST_UNSIGNED_LONG_LONG:
        snprintf(destination, max, "%lld", *((unsigned long long*) content));
        break;
      case LIST_FLOAT:
        snprintf(destination, max, "%f", *((float*) content));
        break;
      case LIST_DOUBLE:
        snprintf(destination, max, "%lf", *((double*) content));
        break;
      case LIST_LONG_DOUBLE:
        snprintf(destination, max, "%Lf", *((long double*) content));
        break;
      case LIST_STRING:
        strncpy(destination, content, max);
        break;
    }
  }
  else
  {
    void (*customToString) (void*, char*, int);
    customToString = list->toString;
    (*customToString) (content, destination, max);
  }
}

void list_toString(struct List* list, int index, char* destination, int max) {
  list_elementContentToString(list, list_elementInternal(list, index), destination, max);
}

void list_prettyPrintElement(struct List* list, struct ListElement* given_element) {
  printf("  # Element %p\n", given_element);
  printf("     - next:     %p\n", given_element->nextElement);
  printf("     - previous: %p\n", given_element->previousElement);
  if (given_element->content == LIST_UNDEFINED)
  {
    puts("     - content:  undefined\n");
  }
  else
  {
    printf("     - content:  %p\n", given_element->content);
    char contentString[LIST_STRING_SIZE] = "";
    list_elementContentToString(list, given_element, contentString, LIST_STRING_SIZE);
    printf("     - toString: %s\n\n", contentString);
  }
}

/**
  Vergleicht zwei Listenelemente miteinander
*/
int list_compareElements(struct List* list, int index_a, int index_b) {
  void* a = list_element(list, index_a);
  void* b = list_element(list, index_b);
  if (list->type != LIST_USER_DEFINED && list->compare == LIST_UNDEFINED)
  {
    switch (list->type)
    {
      case LIST_BOOL:
        if ((*((int*) a) == 0 && *((int*) b) == 0) || (*((int*) a) != 0 && *((int*) b) != 0))
        {
          return 0;
        }
        else if (*((int*) b) == 0)
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_CHAR:
        if (*((char*) a) == *((char*) b))
        {
          return 0;
        }
        else if (*((char*) a) > *((char*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_SIGNED_CHAR:
        if (*((signed char*) a) == *((signed char*) b))
        {
          return 0;
        }
        else if (*((signed char*) a) > *((signed char*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_UNSIGNED_CHAR:
        if (*((unsigned char*) a) == *((unsigned char*) b))
        {
          return 0;
        }
        else if (*((unsigned char*) a) > *((unsigned char*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_SHORT:
        if (*((short*) a) == *((short*) b))
        {
          return 0;
        }
        else if (*((short*) a) > *((short*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_UNSIGNED_SHORT:
        if (*((unsigned short*) a) == *((unsigned short*) b))
        {
          return 0;
        }
        else if (*((unsigned short*) a) > *((unsigned short*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_INT:
        if (*((int*) a) == *((int*) b))
        {
          return 0;
        }
        else if (*((int*) a) > *((int*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_UNSIGNED_INT:
        if (*((unsigned int*) a) == *((unsigned int*) b))
        {
          return 0;
        }
        else if (*((unsigned int*) a) > *((unsigned int*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_LONG:
        if (*((long*) a) == *((long*) b))
        {
          return 0;
        }
        else if (*((long*) a) > *((long*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_UNSIGNED_LONG:
        if (*((unsigned long*) a) == *((unsigned long*) b))
        {
          return 0;
        }
        else if (*((unsigned long*) a) > *((unsigned long*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_LONG_LONG:
        if (*((long long*) a) == *((long long*) b))
        {
          return 0;
        }
        else if (*((long long*) a) > *((long long*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_UNSIGNED_LONG_LONG:
        if (*((unsigned long long*) a) == *((unsigned long long*) b))
        {
          return 0;
        }
        else if (*((unsigned long long*) a) > *((unsigned long long*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_FLOAT:
        if (*((float*) a) == *((float*) b))
        {
          return 0;
        }
        else if (*((float*) a) > *((float*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_DOUBLE:
        if (*((double*) a) == *((double*) b))
        {
          return 0;
        }
        else if (*((double*) a) > *((double*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_LONG_DOUBLE:
        if (*((long double*) a) == *((long double*) b))
        {
          return 0;
        }
        else if (*((long double*) a) > *((long double*) b))
        {
          return 1;
        }
        else
        {
          return -1;
        }
        break;
      case LIST_STRING:
        return strcmp((char*) a, (char*) b);
        break;
    }
  }
  else
  {
    int (*customCompare) (void*, void*);
    customCompare = list->compare;
    return (*customCompare) (a, b);
  }
  return 0;
}

/**
  Sortiert die Liste
*/
int list_sort(struct List* list) {
  if (list->length > 1)
  {
    for (int i = 1; i < list->length; i++)
    {
      for (int j = 0; j < i; j++)
      {
        if (list_compareElements(list, j, i) == 1) {
          list_insertBefore(list, j, list_element(list, i));
          list_remove(list, i + 1);
        }
      }
    }
  }
  return LIST_SUCCESS;
}

void list_prettyPrint(struct List* list) {
  char stringCache[LIST_STRING_CACHE_SIZE] = "";
  
  printf("\n# List (Header-Position: %p)\n\n", list);
  
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
  printf("  - A compare()-callback is %s defined.\n\n", stringCache);
  
  if (list->firstElement == LIST_UNDEFINED)
    puts("# First Element is undefined.\n\n");
  else
    printf("# First Element at: %p\n\n", list->firstElement);
  
  puts(" - Crawling elements..\n");
  if (list->length > 0)
  {
    struct ListElement* element;
    for (int i = 0; i < list->length; i++)
    {
      element = list_elementInternal(list, i);
      list_prettyPrintElement(list, element);
    }
  }
  
  puts("\n");
}
