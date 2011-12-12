#include <stdlib.h>
#include <stdbool.h>

typedef unsigned int uint ;



typedef struct
{
    char* _lexem ;
    uint  _line ;
    uint  _column ;
} Token ;

void* copyToken(const void* tok) ;

const int n = 17 ;