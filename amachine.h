enum AMCH_CONSTANTS {
  AMCH_OFF = 0
};

struct AMachineState {
  int currentChar;
  int previousChar;
  int currentNode;
  unsigned int line;
  unsigned int column;
  FILE* file;
};

struct AMachineToken {
  char* token;
  unsigned int line;
  unsigned int column;
};