#ifndef LEX_UTIL_H
#define LEX_UTIL_H

#define RANGE(c, l, h) ((c) >= (l) && (c) <= (h))
#define IS_IDENT1(c) (RANGE(c, 'A', 'Z') || RANGE(c, 'a', 'z') || (c) == '_')
#define IS_IDENT2(c) (IS_IDENT1(c) || RANGE(c, '0', '9'))
#define IS_HEX(c) (RANGE(c, '0', '9') || RANGE(c, 'a', 'f') || RANGE(c, 'A', 'F'))

// fixed values; rest is user-defined
#define T_EOF 0
#define T_ERROR 1

typedef struct token Token;
typedef struct lexer Lexer;

struct positon {
    size_t off;
    size_t lineno;
    size_t lineoff;
};

struct token {
    int type;
    const char *s;
    size_t len;
    struct position pos;
};

struct lexer {
    Token tok;
    
    struct position curr, comm;
    
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
