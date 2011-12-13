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
void amch_token_toString(struct AMachineToken* token, char* destination, int max);
void amch_token_destroyContent(struct AMachineToken* token);
int amch_token_compare(struct AMachineToken* token_a, struct AMachineToken* token_b);
struct AMachineToken* amch_token_setContent(struct AMachineToken* token);