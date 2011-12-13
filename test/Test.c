#include <stdlib.h>
#include <stdbool.h>
// Dies ist ein Zeilenkommentar.
typedef unsigned int uint ;
/** Dies ist die Definition des 
 *  strukturierten Typs "Token"
 */
typedef struct
{
    char* _lexem ;
    uint  _line ;
    uint  _column ;
} Token ;

void* copyToken(const void* tok) ;

const int n = 17 / 8;