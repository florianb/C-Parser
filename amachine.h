enum AMCH_CONSTANTS {
  AMCH_OFF  = -1, /// Unterbricht den Automaten sofort
  AMCH_TOKEN_BUFFER_SIZE = 512 /// Größe des Token-Buffers
};

/**
  Datenstruktur eines Automaten, beinhält alle relevanten Statusinformationen
*/
struct AMachineState {
  char* tokenBuffer; /// Tokenbuffer, enthält bisher eingelesene Zeichen
  unsigned int tokenLine; /// Zeile, in der der aktuelle Tokenbuffer begonnen wurde
  unsigned int tokenColumn; /// Spalte, in der der aktuelle TokenBuffer begonnen wurde
  int currentChar; /// Derzeitiges Zeichen
  int previousChar; /// Vorangegangenes Zeichen
  int ignoreComments; /// Schalter: 0 = Ignoriere Kommentare, !0 = Tokenize auch Kommentare
  unsigned int line; /// Aktuelle Zeile
  unsigned int column; /// Aktuelle Spalte
  unsigned int tokenPosition; /// Aktuelle Position im TokenBuffer
  struct List* list; /// Liste in die die Tokens gespeichert werden
  FILE* file; /// Dateihandle, aus dem die Zeichen gelesen werden
};

/**
  Datenstruktur eines Tokens
*/
struct AMachineToken {
  char* content; /// String des Tokens
  unsigned int line; /// Zeile in der das Token gefunden wurde
  unsigned int column; /// Spalte ind er das Token gefunden wurde
};

/**
  Startet Automaten mit der angegebenen Liste und der angegebenen Datei
  
  Die Datei muss bereits lesebereit vorliegen.
*/
void amch_run(struct List** list, FILE** file);

/**
  Callback-Funktion zur Stringrepräsentation eines Tokens innerhalb der Liste
*/
void amch_token_toString(struct AMachineToken* token, char* destination, int max);

/**
  Callback-Funktion um Ein token innerhalb einer Liste zu entfernen
*/
void amch_token_destroyContent(struct AMachineToken* token);

/**
  Callback-Funktion um zwei Tokens lexikograpghisch in der Liste zu vergleichen
*/
int amch_token_compare(struct AMachineToken* token_a, struct AMachineToken* token_b);

/**
  Callback-Funktion um ein Token in die Liste zu kopieren
*/
struct AMachineToken* amch_token_setContent(struct AMachineToken* token);