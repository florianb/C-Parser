enum AMCH_CONSTANTS {
  AMCH_OFF = -1,
  
  AMCH_TOKEN_BUFFER_SIZE = 512
};

struct AMachineState {
  char* tokenBuffer;
  unsigned int tokenLine;
  unsigned int tokenColumn;
  int currentChar;
  int previousChar;
  int ignoreComments;
  unsigned int line;
  unsigned int column;
  unsigned int tokenPosition;
  struct List* list;
  FILE* file;
};

struct AMachineToken {
  char* content; //8
  unsigned int line; //4
  unsigned int column; //4
};

void amch_run(struct List** list, FILE** file);