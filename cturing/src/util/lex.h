#ifndef LEX_H
#define LEX_H

#include <stddef.h>

// fixed values; rest is user-defined
#define T_EOF 0
#define T_ERROR 1
#define T_SYM 2
#define T_IDENT 3

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

/**
 * Print gcc-style error to stderr.
 * @param tok the token that caused the error
 * @param msg the error message
 */
void Token_error(const Token *tok, const char *msg);

/**
 * Initialize lexer.
 * @param lx the lexer
 * @param s the input string
 */
void Lexer_init(Lexer *lx, const char *s);

/**
 * Generate another token from lexer.
 * @param lx the lexer
 * @return token type, equal to lx->tok.type
 */
int lex(Lexer *lx);

#endif
