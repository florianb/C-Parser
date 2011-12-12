enum AMCH_CONSTANTS {
  AMCH_OFF = 0,
  
  AMCH_TOKEN_BUFFER_SIZE = 512
};

struct AMachineState {
  char* tokenBuffer;
  int currentChar;
  int previousChar;
  int currentNode;
  unsigned int line;
  unsigned int column;
  unsigned int tokenPosition;
  FILE* file;
};

struct AMachineToken {
  char* token;
  unsigned int line;
  unsigned int column;
};

void amch_run(struct List* list, FILE* file);