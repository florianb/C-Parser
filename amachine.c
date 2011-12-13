#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "list.h"
#include "amachine.h"

/**
  Initialisiert den Automaten
*/
void amch_initialize(struct AMachineState* machine, FILE* file)
{
  //machine->tokenBuffer = malloc(AMCH_TOKEN_BUFFER_SIZE);
  /*for (int i = 0; i < AMCH_TOKEN_BUFFER_SIZE; i++)
  {
    *(machine->tokenBuffer + i) = '\0';
  }*/
  
  machine->tokenBuffer = malloc(1);
  *(machine->tokenBuffer) = '\0';
  machine->currentChar = -2;
  machine->previousChar = -2;
  machine->currentNode = AMCH_OFF;
  machine->line = 1;
  machine->column = 1;
  machine->tokenPosition = 0;
  machine->file = file;
  rewind(machine->file);
  puts("Abstract machine initialized..");
}

/**
  Zerstört den Automaten
*/
void amch_destroy(struct AMachineState* machine)
{
  puts("Destroying abstract machine..");
  fclose(machine->file);
  free(machine->tokenBuffer);
  free(machine);
}

/**
  Liest ein einzelnes Zeichen aus der Datei ein
*/
void amch_readChar(struct AMachineState* machine)
{
  machine->previousChar = machine->currentChar;
  machine->currentChar = fgetc(machine->file);
}

/**
  Setzt den TokenBuffer zurück
*/
void amch_resetTokenBuffer(struct AMachineState* machine)
{
  /*for (int i = 0; i <= machine->tokenPosition; i++)
  {
    *(machine->tokenBuffer + i) = '\0';
  }*/
  machine->tokenBuffer = realloc(machine->tokenBuffer, 1);
  if (machine->tokenBuffer == NULL)
  {
    puts("AMCH: Fehler: TokenBuffer konnte nicht reallokiert werden.");
    amch_destroy(machine);
    exit(1);
  }
  machine->tokenBuffer[0] = '\0';
  machine->tokenPosition = 0;
}

/**
  Fügt dem TokenBuffer in einzelnes Zeichen hinzu
*/
void amch_chargeTokenBuffer(struct AMachineState* machine, char nextChar)
{
  int tokenSize = strlen(machine->tokenBuffer);
  machine->tokenBuffer = realloc(machine->tokenBuffer, tokenSize + 2);
  if (machine->tokenBuffer == NULL)
  {
    puts("AMCH: Fehler: TokenBuffer konnte nicht reallokiert werden.");
    amch_destroy(machine);
    exit(1);
  }
  machine->tokenBuffer[tokenSize] = nextChar;
  machine->tokenBuffer[tokenSize + 1] = '\0';
  machine->tokenPosition = tokenSize;
}

/**
  Aktualisiert Spalten und Zeilenangaben
*/
void amch_updatePosition(struct AMachineState* machine)
{
  if (machine->currentChar == '\n')
  {
    printf("Zeile %d nach %d Zeichen abgeschlossen.\n", machine->line, machine->column);
    machine->line++;
    machine->column = 0;
  }
  machine->column++;
}

void amch_token_toString(struct AMachineToken* token, char* destination, int max)
{
  //struct AMachineToken* token = element->content;
  snprintf(destination, max, "(%s, %d, %d)\n", token->content, token->line, token->column);
}

struct AMachineToken* amch_token_setContent(struct AMachineToken* token)
{
  struct AMachineToken* new_token;
  printf("(%s, %d, %d)\n", token->content, token->line, token->column);
  
  int size = strlen(token->content) + 1;
  
  size += sizeof(struct AMachineToken);
  new_token = malloc(size);
  if (new_token == NULL)
  {
    puts("AMCH: setContent: Fehler: Neuer Token konnte nicht reallokiert werden.");
    exit(1);
  }
  new_token->line = token->line;
  new_token->column = token->column;
  new_token->content = (char*) &new_token[1];
  
  printf("Token at %p content at %p: diff %d\n", new_token, new_token->content, (int) new_token->content - (int) new_token);
  
  new_token->content[0] = '\0';
  
  printf("Allocated %d bytes at %p..\n", size, new_token);
  
  printf("Prepared new token %p (Size: %d, %s, %d, %d)..\n", new_token, size, new_token->content, new_token->line, new_token->column);

  
  strcpy(new_token->content, token->content);
  printf("Finished new token %p (Size: %d, %p, %d, %d)..\n", new_token, size, new_token->content, new_token->line, new_token->column);
  return new_token;
}

void amch_token_destroyContent(struct AMachineToken* token)
{
  printf("Freeing custom content at %p..\n", token);
  free(token);
}

struct AMachineToken* amch_token_create()
{
  struct AMachineToken* token = malloc(sizeof(struct AMachineToken) + 1);
  token->content = (char*) &token[1];
  
  printf("Declared token %p (size %lu bytes) with string at %p..\n", token, sizeof(struct AMachineToken) + 1, token->content);
  
  token->content[0] = '\0';
  token->line = 0;
  token->column = 0;
  return token;
}

void amch_run(struct List** list_ptr, FILE** file_ptr)
{
  struct List* list = *list_ptr;
  FILE* file = *file_ptr;
  
  struct AMachineState* machine = malloc(sizeof(struct AMachineState));
  amch_initialize(machine, file);
  
  list = list_create(LIST_USER_DEFINED + sizeof(struct AMachineToken));
  *list_ptr = list;
  list->toString = &amch_token_toString;
  list->setContent = &amch_token_setContent;
  list->destroyContent = &amch_token_destroyContent;
  
  struct AMachineToken* token = amch_token_create();
  
  while (feof(machine->file) == 0)
  {
    amch_readChar(machine);
    if (machine->currentChar != ' ' &&
        machine->currentChar != '\n' &&
        machine->currentChar != '\r' &&
        machine->currentChar != '\0' &&
        machine->currentChar != '\t')
    {
      amch_chargeTokenBuffer(machine, machine->currentChar);
    }
    
    if (machine->currentChar == '\n')
    {
      //token->line = machine->line;
      //token->column = machine->column;
      //token->content = machine->tokenBuffer;  
      printf("Current TokenBuffer: %s (token->content: %s, length: %d)\n", machine->tokenBuffer, token->content, strlen(token->content));
      list_insertAfter(list, -1, (struct AMachineToken*) machine);
      
      amch_resetTokenBuffer(machine);
      //list_insertAfter(list, -1, (struct AMachineToken*) machine);
    }
    
    
    
    amch_updatePosition(machine);
  }
  
  printf("\n# TokenBuffer:\n\n%s\n\n\n", machine->tokenBuffer);
  
  free(token);
  amch_destroy(machine);
  puts("Machine killed.");
}