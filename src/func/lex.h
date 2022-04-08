#ifndef TURING_LEX_H
#define TURING_LEX_H

#define T_EOF 0
#define T_ERROR 1
#define T_IDENT 2
#define T_LCURLY '{'
#define T_RCURLY '}'
#define T_LPAR '('
#define T_RPAR ')'
#define T_LBRA '['
#define T_RBRA ']'
#define T_COMMA ','

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
void lex(struct lex *lx);
void expect(struct lex *lx, char type);

#endif
