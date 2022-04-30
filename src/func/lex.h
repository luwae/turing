#ifndef TURING_LEX_H
#define TURING_LEX_H

#ifdef __cplusplus
extern "C" {
#endif


#define LT_EOF 0
#define LT_ERROR 1
#define LT_IDENT 2
#define LT_CHR_IMM 3
#define LT_CHR_VAR 4
#define LT_PRINT '='
#define LT_MOVER '>'
#define LT_MOVEL '<'
#define LT_LCURLY '{'
#define LT_RCURLY '}'
#define LT_LPAR '('
#define LT_RPAR ')'
#define LT_LBRA '['
#define LT_RBRA ']'
#define LT_COMMA ','

struct token {
    char type;
    int len;
    char *s;
};

struct lex {
    char *input;
    char *s;
    struct token cur;
};

void lex_init(struct lex *lx, char *input);
void lex_reset(struct lex *lx);
void lex(struct lex *lx);
void expect(struct lex *lx, char type);


#ifdef __cplusplus
}
#endif

#endif
