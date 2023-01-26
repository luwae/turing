#ifndef UTIL_LEX_STRUCTS_H
#define UTIL_LEX_STRUCTS_H

#include <stddef.h>

// fixed values; rest is user-defined
#define T_EOF 0
#define T_ERROR 1

struct position {
    size_t off, lineno, lineoff;
};

typedef struct token Token;
struct token {
    int type;
    const char *s;
    size_t len;
    struct position pos;
};

typedef struct lexer Lexer;
struct lexer {
    Token tok;
    
    struct position curr, comm;
    size_t neof;
    
    const char *s;
    int done;
};

#endif
