#ifndef LEX_H
#define LEX_H

#define T_EOF 0
#define T_ERROR 1
#define T_SYM 2
#define T_IDENT 3
#define T_MOVEL 4
#define T_MOVER 5
#define T_PRINT 6
#define T_LCURLY 7
#define T_RCURLY 8
#define T_LBRACKET 9
#define T_RBRACKET 10
#define T_COMMA 11
#define T_DEF 12
#define T_ACCEPT 13
#define T_REJECT 14
#define T_RANGE 15

typedef struct token Token;
typedef struct lexer Lexer;

struct token {
    int type;
    const char *s;
    size_t off;
    size_t len;
    size_t lineno;
    size_t lineoff;
};

struct lexer {
    Token tok;
    
    size_t off;
    size_t len;
    size_t lineno;
    size_t lineoff;
    
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
 * @return token type, equal to toktype()
 */
int lex(Lexer *lx);

#endif
