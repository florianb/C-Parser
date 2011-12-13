#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "amachine.h"

/**
  Datenstruktur für den Kopfbereich eines XRef-Elementes
*/
struct XRefHead {
  char* token; /// String-Zeiger auf den Token des XRef-Elementes
  int references; /// Anzahl der Referenzen innerhalb des XRef-Elementes
};

/**
  Datenstruktur für eine Referenz innerhalb eines XRef-Elementes
*/
struct XRefReference {
  int line;
  int column;
};

/**
  Erstellt ein XRef-Element aus einem AMachineToken
*/
struct XRefHead* xref_create(struct AMachineToken* token)
{
  int size =  sizeof(struct XRefHead) +
              sizeof(struct XRefReference) +
              strlen(token->content) + 1;
  struct XRefHead* head = malloc(size);
  head->token = ((char*) &head[1]);
  head->references = 1;
  strcpy(head->token, token->content);
  printf("XRef: %p (Size: %d bytes, Token at %p with %lu bytes)\n", head, size, head->token, strlen(head->token));
  memcpy((void*) head->token[strlen(head->token)],
         (void*) token->content[strlen(head->token)],
         sizeof(struct XRefReference));
  return head;
}

/**
  Fügt einem XRef-Element eine weitere Referenz aus einem AMachineToken hinzu
*/
struct XRefHead* xref_addReference(struct XRefHead* head, struct AMachineToken* token)
{
  head->references++;
  int size =  sizeof(struct XRefHead) +
              (sizeof(struct XRefReference) * head->references) +
              strlen(head->token) + 1;
  printf("O-XRef: %p (Size: %d bytes, Token at %p with %lu bytes)\n", head, size, head->token, strlen(head->token));

  head = realloc(head, size);
  struct XRefReference reference = {token->line, token->column};
  printf("R-XRef: %p (Size: %d bytes, Token at %p with %lu bytes)\n", head, size, head->token, strlen(head->token));
  memcpy( (void*) head->token[strlen(head->token) +
          ((head->references - 1) * sizeof(struct XRefReference))],
          (void*) &token->line,
          sizeof(struct XRefReference));
  return head;
}

/**
  Gibt ein Element einer Xref-Liste in einen String aus
*/
void xref_toString(struct XRefHead* head, char* destination, int max)
{
  sprintf(destination, "(%s", head->token);
  struct XRefReference* reference;
  for (int i = 0; i < head->references; i++)
  {
    reference = (struct XRefReference*) head->token[strlen(head->token) + 2 +
                                        (sizeof(struct XRefReference) * i)];
    sprintf(&destination[strlen(destination)],
            " (%d,%d)",
            reference->line,
            reference->column);
  }
  sprintf(&destination[strlen(destination)], ")\n");
}

void amch_token_toString(struct AMachineToken* token, char* destination, int max);

/**
  Callback-Funktion um ein XRef-Element innerhalb einer Liste zu entfernen
*/
void xref_destroyContent(struct XRefHead* head)
{
  free(head);
}

/**
  Callback-Funktion um ein Token in die Liste zu kopieren
*/
struct XRefHead* xref_setContent(struct XRefHead* head)
{
  int size = sizeof(struct XRefHead) + (sizeof(struct XRefReference) *
                                    head->references) + strlen(head->token) + 1;
  struct XRefHead* new_head = malloc(size);
  memcpy(new_head, head, size);
  return new_head;
}

int main() {
  char test[512] = "";
  puts("\nparser 0.01 - 2011 by florianbreisch@b-lebt.de\n\n");

  
  struct List* list;
  struct List* list2;
  
  FILE* file = fopen("test/Test.c", "r");
  amch_run(&list, &file); // Startet den Automaten
  //list_prettyPrint(list);
  
  printf("Liste mit %d Elementen erstellt.\n", list->length);
  list2 = list_create(LIST_USER_DEFINED + sizeof(struct AMachineToken));
  list2->toString = &amch_token_toString;
  list2->setContent = &amch_token_setContent;
  list2->destroyContent = &amch_token_destroyContent;
  list2->compare = &amch_token_compare;
  
  /// Kopiert alle Elemente die mit [a-zA-Z_] beginnen in eine neue Liste
  for (int i = 0; i < list->length; i++)
  {
    list_toString(list, i, test, 511);
    //printf("test: %c\n", test[2]);
    if (('a' <= test[2] && test[2] <= 'z') ||
      ('A' <= test[2] && test[2] <= 'Z') ||
      test[2] == '_'
    )
    {
      list_insertAfter(list2, 0, list_element(list, i));
    }
  }
  
  printf("Liste2 mit %d Elementen erstellt.\n", list2->length);
  
  list_sort(list2);
  
  list_destroy(list);
  list = list_create(LIST_USER_DEFINED);
  list->setContent = &xref_setContent;
  list->destroyContent = &xref_destroyContent;
  list->toString = &xref_toString;
  
  list_insertAfter(list, 0, xref_create(list_element(list2, 0)));
  
  for (int i = 1; i < list2->length; i++)
  {
    list_toString(list2, i, test, 511);
    
    if (
    strcmp(
      ((struct XRefHead*) list_element(list, -1))->token,
      ((struct AMachineToken*) list_element(list2, i))->content) == 0)
    {
      list_setContent(list, -1, xref_addReference(
            (struct XRefHead*) list_element(list, -1),
            (struct AMachineToken*) list_element(list2, i)));
    }
    else
    {
      list_insertAfter(list, -1, xref_create(list_element(list2, i)));
    }
    printf("%s\n", test);
  }
  
  list_destroy(list);
  list_destroy(list2);
  return 0;
}