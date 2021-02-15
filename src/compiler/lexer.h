#ifndef LEXER_H
#define LEXER_H

/**
 * Possible token types. EOF is a separate type.
 */
#define T_EOF 0
#define T_STATEDEF 1
#define T_SYMDEF 2
#define T_SYM 3
#define T_MOVE 4
#define T_STATE 5

/**
 * Token names, for debug printing.
 */
char *tokennames[] = {"EOF", "STATEDEF", "SYMDEF", "SYM", "MOVE", "STATE"};

/**
 * One token, containing type information and a pointer to its substring.
 */
struct token {
    int type;
    int len;
    char *content;
};

/**
 * A lexer instance.
 */
struct lexer_state {
    int terminated;
    char *s;
};

void lexer_init(struct lexer_state *state, char *s);
int lexer_gettoken(struct lexer_state *state, struct token *t);

#endif
