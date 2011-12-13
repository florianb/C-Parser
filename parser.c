#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "amachine.h"

struct XRefHead {
  char* token;
  int references;
};

struct XRefReference {
  int line;
  int column;
};

struct XRefHead* xref_create(struct AMachineToken* token)
{
  struct XRefHead* head = malloc(sizeof(struct XRefHead) + sizeof(struct XRefReference) + strlen(token->content) + 1);
  head->token = ((char*) &head[1]);
  head->references = 1;
  strcpy(head->token, token->content);
  head->token[strlen(token->content)] = '\0';
  struct XRefReference reference = {token->line, token->column};
  memcpy((void*) head->token[strlen(token->content) + 2], &reference, sizeof(struct XRefReference));
  return head;
}

struct XRefHead* xref_addReference(struct XRefHead* head, struct AMachineToken* token)
{
  head->references++;
  head = realloc(head, sizeof(struct XRefHead) + (sizeof(struct XRefReference) * head->references) + strlen(head->token) + 1);
  struct XRefReference reference = {token->line, token->column};
  memcpy((void*) head->token[strlen(token->content) + 2 + (sizeof(struct XRefReference) * head->references)], &reference, sizeof(struct XRefReference));
  return head;
}

void xref_toString(struct XRefHead* head, char* destination)
{
  sprintf(destination, "(%s", head->token);
  struct XRefReference* reference;
  for (int i = 0; i < head->references; i++)
  {
    reference = (struct XRefReference*) head->token[strlen(head->token) + 2 + (sizeof(struct XRefReference) * i)];
    sprintf(&destination[strlen(destination)], " (%d,%d)", reference->line, reference->column);
  }
  sprintf(&destination[strlen(destination)], ")\n");
}

int main() {
  char test[512] = "";
  puts("\nparser 0.01 - 2011 by florianbreisch@b-lebt.de\n\n");

  
  struct List* list;
  struct List* list2;
  
  FILE* file = fopen("test/gcc2.c", "r");
  amch_run(&list, &file);
  //list_prettyPrint(list);
  
  printf("Liste mit %d Elementen erstellt.\n", list->length);
  list2 = list_create(LIST_USER_DEFINED + sizeof(struct AMachineToken));
  list2->toString = &amch_token_toString;
  list2->setContent = &amch_token_setContent;
  list2->destroyContent = &amch_token_destroyContent;
  list2->compare = &amch_token_compare;
  
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
  
  for (int i = 0; i < list2->length; i++)
  {
    list_toString(list2, i, test, 511);
    printf("%s\n", test);
  }
  
  list_destroy(list);
  list_destroy(list2);
  return 0;
}