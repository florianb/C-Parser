#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "list.h"
#include "amachine.h"

/**
  Initialisiert den Automaten
*/
void amch_initialize(struct AMachineState* machine, struct List* list, FILE* file)
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
  machine->line = 1;
  machine->column = 1;
  machine->tokenLine = 1;
  machine->tokenColumn = 1;
  machine->ignoreComments = 0;
  machine->tokenPosition = 0;
  machine->file = file;
  machine->list = list;
  rewind(machine->file);
  //puts("Abstract machine initialized..");
}

/**
  Zerstört den Automaten
*/
void amch_destroy(struct AMachineState* machine)
{
  //puts("Destroying abstract machine..");
  fclose(machine->file);
  free(machine->tokenBuffer);
  free(machine);
}

/**
  Aktualisiert Spalten und Zeilenangaben
*/
void amch_updatePosition(struct AMachineState* machine)
{
  if (machine->currentChar == '\n')
  {
    //printf("Zeile %d nach %d Zeichen abgeschlossen.\n", machine->line, machine->column);
    machine->line++;
    machine->column = 0;
  }
  machine->column++;
}

/**
  Liest ein einzelnes Zeichen aus der Datei ein
*/
void amch_readChar(struct AMachineState* machine)
{
  amch_updatePosition(machine);
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
  Callback-Funktion zur Stringrepräsentation eines Tokens innerhalb der Liste
*/
void amch_token_toString(struct AMachineToken* token, char* destination, int max)
{
  //struct AMachineToken* token = element->content;
  snprintf(destination, max, "(\"%s\", %d, %d)\n", token->content, token->line, token->column);
}

/**
  Callback-Funktion um ein Token in die Liste zu kopieren
*/
struct AMachineToken* amch_token_setContent(struct AMachineToken* token)
{
  struct AMachineToken* new_token;
  //printf("(\"%s\", %d, %d)\n", token->content, token->line, token->column);
  
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
  
  //printf("Token at %p content at %p: diff %d\n", new_token, new_token->content, (int) new_token->content - (int) new_token);
  
  new_token->content[0] = '\0';
  
  //printf("Allocated %d bytes at %p..\n", size, new_token);
  
  //printf("Prepared new token %p (Size: %d, %s, %d, %d)..\n", new_token, size, new_token->content, new_token->line, new_token->column);

  
  strcpy(new_token->content, token->content);
  //printf("Finished new token %p (Size: %d, %p, %d, %d)..\n", new_token, size, new_token->content, new_token->line, new_token->column);
  return new_token;
}

/**
  Callback-Funktion um Ein token innerhalb einer Liste zu entfernen
*/
void amch_token_destroyContent(struct AMachineToken* token)
{
  //printf("Freeing custom content at %p..\n", token);
  free(token);
}

/**
  Callback-Funktion um zwei Tokens lexikograpghisch in der Liste zu vergleichen
*/
int amch_token_compare(struct AMachineToken* token_a, struct AMachineToken* token_b)
{
  return strcmp(token_a->content, token_b->content);
}

/**
  Erstellt einen leeren Token
*/
struct AMachineToken* amch_token_create()
{
  struct AMachineToken* token = malloc(sizeof(struct AMachineToken) + 1);
  token->content = (char*) &token[1];
  
  //printf("Declared token %p (size %lu bytes) with string at %p..\n", token, sizeof(struct AMachineToken) + 1, token->content);
  
  token->content[0] = '\0';
  token->line = 0;
  token->column = 0;
  return token;
}

/**
  Schließt die Erfassung eines Tokens ab und speichert ihn in der Liste
*/
void amch_finishToken(struct AMachineState* machine)
{
  list_insertAfter(machine->list, -1, (struct AMachineToken*) machine);
  amch_resetTokenBuffer(machine);
}

/**
  Dummy-Knoten für den Automaten-Graphen
*/
int amch_000(struct AMachineState* machine)
{
  return AMCH_OFF;
}

/**
  Haupt-Knoten des Automaten-Graphen
*/
int amch_100(struct AMachineState* machine)
{
  if (machine->currentChar == ' ' ||
      machine->currentChar == '\n' ||
      machine->currentChar == '\r' ||
      machine->currentChar == '\0' ||
      machine->currentChar == '\t')
  {
    return 100;
  }
  else if (
    ('a' <= machine->currentChar && machine->currentChar <= 'z') ||
    ('A' <= machine->currentChar && machine->currentChar <= 'Z') ||
    machine->currentChar == '_'
  )
  {
    machine->tokenLine = machine->line;
    machine->tokenColumn = machine->column;
    amch_chargeTokenBuffer(machine, machine->currentChar);
    return 200;
  }
  else if (
    ('0' <= machine->currentChar && machine->currentChar <= '9')
  )
  {
    machine->tokenLine = machine->line;
    machine->tokenColumn = machine->column;
    amch_chargeTokenBuffer(machine, machine->currentChar);
    return 300;
  }
  else if (
    machine->currentChar == '/' && machine->ignoreComments != 0
  )
  {
    machine->tokenLine = machine->line;
    machine->tokenColumn = machine->column;
    return 400;
  }
  machine->tokenLine = machine->line;
  machine->tokenColumn = machine->column;
  amch_chargeTokenBuffer(machine, machine->currentChar);
  amch_finishToken(machine);
  return 100;
}

/**
  Automaten-Knoten 200: Fortführende Erkennung von Strings
*/
int amch_200(struct AMachineState* machine)
{
  if (
    ('a' <= machine->currentChar && machine->currentChar <= 'z') ||
    ('A' <= machine->currentChar && machine->currentChar <= 'Z') ||
    ('0' <= machine->currentChar && machine->currentChar <= '9') ||
    machine->currentChar == '_'
  )
  {
    amch_chargeTokenBuffer(machine, machine->currentChar);
    return 200;
  }
  amch_finishToken(machine);
  return 10;
}

/**
  Automaten-Knoten 300: Fortführende Erkennung von Zahlen
*/
int amch_300(struct AMachineState* machine)
{
  if (
    ('0' <= machine->currentChar && machine->currentChar <= '9')
  )
  {
    amch_chargeTokenBuffer(machine, machine->currentChar);
    return 300;
  }
  else if (machine->currentChar == '.')
  {
    amch_chargeTokenBuffer(machine, machine->currentChar);
    return 301;
  }
  amch_finishToken(machine);
  return 10;
}

/**
  Automaten-Knoten 301: Ende der Erkennung von Zahlen
*/
int amch_301(struct AMachineState* machine)
{
  if (
    ('0' <= machine->currentChar && machine->currentChar <= '9')
  )
  {
    amch_chargeTokenBuffer(machine, machine->currentChar);
    return 301;
  }
  amch_finishToken(machine);
  return 10;
}

/**
  Automaten-Knoten 400: Fortführende Erkennung von Kommentaren
*/
int amch_400(struct AMachineState* machine)
{
  if (machine->currentChar == '/')
  {
    return 401;
  }
  else if (machine->currentChar == '*')
  {
    return 411;
  }
  amch_chargeTokenBuffer(machine, machine->previousChar);
  amch_finishToken(machine);
  return 10;
}

/**
  Automaten-Knoten 401: Beendigung der Erkennung von 1-Zeilen-Kommentaren
*/
int amch_401(struct AMachineState* machine)
{
  if (machine->currentChar != '\n')
  {
    return 401;
  }
  return 100;
}

/**
  Automaten-Knoten 411: Fortführende Erkennung von Mehrzeilen-Kommentaren
*/
int amch_411(struct AMachineState* machine)
{
  if (machine->currentChar != '*')
  {
    return 411;
  }
  return 412;
}

/**
  Automaten-Knoten 412: Beendigung der Erkennung von Mehrzeilen-Kommentaren
*/
int amch_412(struct AMachineState* machine)
{
  if (machine->currentChar != '/')
  {
    return 411;
  }
  return 100;
}

/**
  Startet Automaten mit der angegebenen Liste und der angegebenen Datei
  
  Die Datei muss bereits zum lesen geöffnet sein.
*/
void amch_run(struct List** list_ptr, FILE** file_ptr)
{
  struct List* list = *list_ptr;
  FILE* file = *file_ptr;
  list = list_create(LIST_USER_DEFINED + sizeof(struct AMachineToken));
  *list_ptr = list;  
  list->toString = &amch_token_toString;
  list->setContent = &amch_token_setContent;
  list->destroyContent = &amch_token_destroyContent;
  list->compare = &amch_token_compare;
  
  struct AMachineState* machine = malloc(sizeof(struct AMachineState));
  amch_initialize(machine, list, file);
  
  int nextStep = 100;
  
  machine->ignoreComments = 1;
  
  while (feof(machine->file) == 0 && nextStep > 0)
  {
    if (nextStep >= 100)
    {
      amch_readChar(machine);
    }
    else
    {
      nextStep *= 10;
    }
      
    switch (nextStep)
    {
      case 100:
        nextStep = amch_100(machine);
        break;
      case 200:
        nextStep = amch_200(machine);
        break;
      case 300:
        nextStep = amch_300(machine);
        break;
      case 301:
        nextStep = amch_301(machine);
        break;
      case 400:
        nextStep = amch_400(machine);
        break;
      case 401:
        nextStep = amch_401(machine);
        break;
      case 411:
        nextStep = amch_411(machine);
        break;
      case 412:
        nextStep = amch_412(machine);
        break;
      default:
        puts("AMachine: Fehler: Undefinierter Zustand");
        list_destroy(machine->list);
        amch_destroy(machine);
        exit(1);
    }
  }
  
  amch_destroy(machine);
}