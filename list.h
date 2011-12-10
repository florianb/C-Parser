/**
  Konstanten-Liste aller Basisdatentypen
  
  Wird benutzt um den Datentyp der Liste festzulegen.
*/
enum LIST_CONSTANTS {
  LIST_USER_DEFINED        = 0,
  LIST_BOOL                = -1,
  LIST_CHAR                = -2,
  LIST_SIGNED_CHAR         = -3,
  LIST_UNSIGNED_CHAR       = -4,
  LIST_SHORT               = -5,
  LIST_UNSIGNED_SHORT      = -6,
  LIST_INT                 = -7,
  LIST_UNSIGNED_INT        = -8,
  LIST_LONG                = -9,
  LIST_UNSIGNED_LONG       = -10,
  LIST_LONG_LONG           = -11,
  LIST_UNSIGNED_LONG_LONG  = -12,
  LIST_FLOAT               = -13,
  LIST_DOUBLE              = -14,
  LIST_LONG_DOUBLE         = -15,
  LIST_STRING              = -16,
  
  LIST_FALSE               = 0,
  LIST_FAILURE             = 0,  
  LIST_TRUE                = 1,
  LIST_SUCCESS             = 1,
  
  LIST_UNDEFINED           = 0, /// Konstante für ungesetzte Zeiger
  LIST_STRING_CACHE_SIZE   = 32, /// Größe eines Cache-Strings
};

/**
  Listen-Element enthält die notwendigen Informationen für eine zweifach, dynamisch verkettete Liste
*/
struct ListElement {
  struct ListElement* previousElement; /// Zeiger auf vorheriges Element der Liste
  struct ListElement* nextElement; /// Zeiger auf nächstes Element der Liste
  void* content; /// Zeiger auf den Inhalt des Elements
};

/**
  Listen-Kopf enthält alle wichtigen Informationen zur Pflege und Benutzung der Liste
*/
struct List {
  unsigned int elementSize; /// Größe eines Elements in Bytes
  unsigned int length; /// Anzahl der Listenelemente
  int type; /// Datentyp der Listenelemente - Wichtig, eine Liste ist immer homogen
  void* toString; /** Callback-Zeiger zu einer Funktion, welche das Element als String-Repräsentation
                      zurückliefert */
  void* compare; /// Callback-Zeiger zu einer Funktion, die zwei Elemente miteinander vergleicht
  struct ListElement* firstElement; // Zeiger auf das erste Listenelement
};

struct List* list_create(int type);
int list_destroy(struct List* list);
void list_prettyPrint(struct List* list);