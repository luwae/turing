/**
 * Converting turing machine source files into token streams.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

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

void token_print(struct token *t) {
    char content[32] = {0};
    switch (t->type) {
        case T_EOF:
        case T_STATEDEF:
            printf("Token(type=%s)\n", tokennames[t->type]);
            break;
        case T_SYMDEF:
        case T_SYM:
        case T_MOVE:
            printf("Token(type=%s, sym=%c)\n", tokennames[t->type], *(t->content));
            break;
        case T_STATE:
            strncpy(content, t->content, t->len);
            printf("Token(type=%s, len=%d, content=%s\n", tokennames[t->type], t->len, content);
            break;
    }
}

/**
 * A lexer instance.
 */
struct lexer_state {
    int terminated;
    char *s;
};

/**
 * Initialize a lexer with an input string.
 */
void lexer_init(struct lexer_state *state, char *s) {
    state->terminated = 0;
    state->s = s;
}

/**
 * Checks if a character is in a given string.
 * @param c the char to test
 * @param s the set of possible characters
 * @return 1 if true
 */
int is_in(char c, char *s) {
    char curr;
    while(curr = *s++) {
        if (curr == c)
            return 1;
    }
    return 0;
}

char *moves = "<>";
char *whitespace = " \n\t";
uint8_t is_sym[] =  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // ' ' is illegal
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, // ':' '<' '>' are illegal
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
    };

#define MISS 0
#define MATCH 1

#define IS_SEP(c) (is_in(c, whitespace) || (c) == '\0')
#define IS_WHITESPACE(c) is_in(c, whitespace)
#define IS_MOVE(c) is_in(c, moves)
#define IN_RANGE(c, low, high) ((c) >= (low) && (c) <= (high))
#define IS_IDENT(c) (IN_RANGE(c, 'a', 'z') || IN_RANGE(c, 'A', 'Z') || IN_RANGE(c, '0', '9') || (c) == '_')
#define IS_SYM(c) is_sym[c]

/**
 * Skip all comments and whitespace characters.
 * @param state the current lexer state
 */
void skip_ahead(struct lexer_state *state) {
    while (1) {
        while (IS_WHITESPACE(*(state->s))) //skip whitespace, but not \0
            state->s++; 
            
        if (*(state->s) == '#') { //skip comment until next line
            while (*(state->s++) != '\n')
                ;
            continue; //check if there is more whitespace to skip after the comment
        }
        
        break;
    }
}

/**
 * Get the next token from a lexer.
 * @param state the current state of the lexer
 * @param t location to store the new token
 * @return 1 if a token has been found, 0 otherwise
 */
int lexer_gettoken(struct lexer_state *state, struct token *t) {
    if (state->terminated)
        return MISS;
    
    skip_ahead(state);
    char *pos = state->s; //first non-trivial character
    
    if(*pos == '\0') {
        state->terminated = 1;
        t->type = T_EOF;
        return MATCH;
    }
    if (strncmp(pos, "state", 5) == 0 && IS_SEP(*(pos + 5))) {
        state->s += 5;
        t->type = T_STATEDEF;
        return MATCH;
    }
    if (IS_MOVE(*pos) && IS_SEP(*(pos + 1))) {
        state->s += 1;
        t->type = T_MOVE;
        t->content = pos;
        return MATCH;
    }
    if (IS_SYM(*pos)) {
        if (*(pos + 1) == ':') {
            state->s += 2;
            t->type = T_SYMDEF;
            t->content = pos;
            return MATCH;
        } else if (IS_SEP(*(pos + 1))) {
            state->s += 1;
            t->type = T_SYM;
            t->content = pos;
            return MATCH;
        }
    }
    
    //get largest possible identifier
    while(IS_IDENT(*pos))
        pos++;
    int ident_size = pos - state->s;
    if (ident_size >= 2 && IS_SEP(*pos)) {
        t->type = T_STATE;
        t->len = ident_size;
        t->content = state->s;
        state->s = pos;
        return MATCH;
    }
    
    return MISS;
}

#ifdef DEBUG
int main() {
    char *s = "#add one to given number.\n\n#move to the right of the word.\nstate s1\n_: < s2\n0: >\n1: >\n\n#exchange 1s for 0s and replace first 0 with 1.\nstate s2\n_: 1 end\n0: 1 end\n1: 0 <";
    struct lexer_state state;
    struct token t;
    
    lexer_init(&state, s);
    
    while (lexer_gettoken(&state, &t)) {
        token_print(&t);
    }
}
#endif
