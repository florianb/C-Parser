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
  machine->column = 0;
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
  if (realloc(machine->tokenBuffer, 1) == NULL)
  {
    puts("AMCH: Fehler: TokenBuffer konnte nicht reallokiert werden.");
    amch_destroy(machine);
    exit(1);
  }
  *(machine->tokenBuffer) = '\0';
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
  *(machine->tokenBuffer + tokenSize) = nextChar;
  *(machine->tokenBuffer + tokenSize + 1) = '\0';
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

void amch_run(struct List* list, FILE* file)
{
  struct AMachineState* machine = malloc(sizeof(struct AMachineState));
  amch_initialize(machine, file);
  
  while (feof(machine->file) == 0)
  {
    amch_readChar(machine);
    
    amch_chargeTokenBuffer(machine, machine->currentChar);
    
    
    amch_updatePosition(machine);
  }
  
  printf("\n# TokenBuffer:\n\n%s\n\n\n", machine->tokenBuffer);
  
  amch_destroy(machine);
}