/**
  Konstanten-Liste aller Basisdatentypen
  
  Wird benutzt um den Datentyp der Liste festzulegen.
*/
enum LIST_DATATYPES {
  USER_DEFINED        = 0,
  BOOL                = -1,
  CHAR                = -2,
  SIGNED_CHAR         = -3,
  UNSIGNED_CHAR       = -4,
  SHORT               = -5,
  UNSIGNED_SHORT      = -6,
  INT                 = -7,
  UNSIGNED_INT        = -8,
  LONG                = -9,
  UNSIGNED_LONG       = -10,
  LONG_LONG           = -11,
  UNSIGNED_LONG_LONG  = -12,
  FLOAT               = -13,
  DOUBLE              = -14,
  LONG_DOUBLE         = -15
};

/// Konstante für ungesetzte Zeiger
const int LIST_UNSET = 0;

/**
  Listen-Kopf enthält alle wichtigen Informationen zur Pflege und Benutzung der Liste
*/
struct ListHeader {
  unsigned int elementSize; /// Größe eines Elements in Bytes
  unsigned int length; /// Anzahl der Listenelemente
  int type; /// Datentyp der Listenelemente - Wichtig, eine Liste ist immer homogen
  void* toString; /** Callback-Zeiger zu einer Funktion, welche das Element als String-Repräsentation
                      zurückliefert */
  void* equals; /// Callback-Zeiger zu einer Funktion, die zwei Elemente miteinander vergleicht
};

/**
  Listen-Element enthält die notwendigen Informationen für eine zweifach, dynamisch verkettete Liste
*/
struct ListElement {
  struct ListElement* previousElement; /// Zeiger auf vorheriges Element der Liste
  struct ListElement* nextElement; /// Zeiger auf nächstes Element der Liste
  void* content; /// Zeiger auf den Inhalt des Elements
};