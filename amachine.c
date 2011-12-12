#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "amachine.h"

void amch_run(struct List* list, FILE* file)
{
  struct AMachineState* machine = malloc(sizeof(struct AMachineState));
  
  machine->currentChar = -2;
  machine->previousChar = -2;
  machine->currentNode = AMCH_OFF;
  machine->line = 1;
  machine->column = 0;
  machine->file = file;
  
  free(machine);
}

void amch_readChar(struct AMachineState* machine)
{
  machine->previousCharacter = machine->currentCharacter;
  machine->currentCharacter = fgetc(machine->file);
  machine->column++;
}