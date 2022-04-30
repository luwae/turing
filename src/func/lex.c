#include <stdio.h>
#include <stdlib.h>

#include "lex.h"

#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\n' || (c) == '\t')
#define IS_ALPHA(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) == '_'))
#define IS_ALPHANUM(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= '0' && (c) <= '9') || ((c) == '_'))
#define IS_VISIBLE(c) ((c) >= ' ' && (c) <= '~')
#define IS_HEX(c) (((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F') || ((c) >= '0' && (c) <= '9'))

void lex_init(struct lex *lx, char *input) {
    lx->input = input;
    lx->s = input;
    lex(lx);
}

void lex_reset(struct lex *lx) {
    lx->s = lx->input;
    lex(lx);
}

int is_in(char c, char *cs) {
    while (*cs)  {
        if (c == *cs++)
            return 1;
    }
    return 0;
}

void skip_whitespace(struct lex *lx) {
    char *s = lx->s;

    for (;;) {
    	while (IS_WHITESPACE(*s)) s++;
    	if (*s == '/') {
    	    if (*(s + 1) == '/') {
    	        s += 2;
    	        while (*s != '\n' && *s != '\0') s++;
    	        if (*s == '\n') s++;
    	    } else if (*(s + 1) == '*') {
    	        s += 2;
    	        for (;;) {
    	            while(*s != '*' && *s != '\0') s++;
    	            if (*s++ == '*') {
    	                if (*s == '/') {
    	                    s++;
    	                    break;
    	                }
    	            } else {
    	                break;
    	            }
    	        }
    	    }
    	} else {
    	    break;
    	}
    }

    lx->s = s;
}

void lex(struct lex *lx) {
    if (lx->cur.type == LT_ERROR || lx->cur.type == LT_EOF)
        return; // no lex after error or eof
    // on LT_ERROR, lx->s should point to the mistaken character

    skip_whitespace(lx);
    char *s = lx->s;

    lx->cur.s = s;
    if (*s == '\0') {
        lx->cur.type = LT_EOF;
    } else if (is_in(*s, "<>{}[](),=")) {
        lx->cur.type = *s++;
        lx->cur.len = 1;
    } else if (*s == '$') {
    	s++;
    	if (*s == '$') {
    	    s++;
    	    lx->cur.type = LT_CHR_VAR;
    	    lx->cur.len = 2;
    	} else if (IS_ALPHA(*s)) {
    	    s++;
    	    while (IS_ALPHANUM(*s)) s++;
    	    lx->cur.type = LT_CHR_VAR;
    	    lx->cur.len = s - lx->cur.s;
    	} else {
    	    lx->cur.type = LT_ERROR;
    	}
    } else if (*s == '\'') {
        s++;
        if (*s == '\'') {
            lx->cur.type = LT_ERROR;
        } else if (*s == 'x') {
            s++;
            if (*s == '\'') {
                lx->cur.type = LT_CHR_IMM;
                lx->cur.len = 3;
            } else if (IS_HEX(*s)) {
                s++;
                if (IS_HEX(*s)) {
                    s++;
                    lx->cur.type = LT_CHR_IMM;
                    lx->cur.len = 5;
                } else {
                    lx->cur.type = LT_ERROR;
                }
            } else {
                lx->cur.type = LT_ERROR;
            }
        } else if (IS_VISIBLE(*s)) {
            s++;
            if (*s == '\'') {
                s++;
                lx->cur.type = LT_CHR_IMM;
                lx->cur.len = 3;
            } else {
                lx->cur.type = LT_ERROR;
            }
        } else {
            lx->cur.type = LT_ERROR;
        }
    } else if (IS_ALPHANUM(*s)) {
        lx->cur.type = LT_IDENT;
        lx->cur.s = s;
        while (IS_ALPHANUM(*s)) s++;
        lx->cur.len = s - lx->cur.s;
    } else {
        lx->cur.type = LT_ERROR;
    }
    lx->s = s;
    return;
}

char *lex_line(struct lex *lx, int *nline) {
    char *target = lx->cur.s;
    char *s = lx->input;
    int lcount = 1;
    char *lstart = lx->input;

    while (s < target) {
        if (*s++ == '\n') {
            lcount++;
            lstart = s;
        }
    }

    if (nline) {
        *nline = lcount;
    }
    return lstart;
}

char *names[] = {
    [0] = "LT_EOF",
    [1] = "LT_ERROR",
    [2] = "LT_IDENT",
    [3] = "LT_CHR_IMM",
    [4] = "LT_CHR_VAR",
    ['='] = "LT_PRINT",
    ['>'] = "LT_MOVER",
    ['<'] = "LT_MOVEL",
    ['{'] = "LT_LCURLY",
    ['}'] = "LT_RCURLY",
    ['('] = "LT_LPAR",
    [')'] = "LT_RPAR",
    ['['] = "LT_LBRA",
    [']'] = "LT_RBRA",
    [','] = "LT_COMMA"
};

void expect(struct lex *lx, char type) {
    if (lx->cur.type != type) {
        if (lx->cur.type == LT_EOF) {
            fprintf(stderr, "lex: unexpected end of file. expected token %s\n", names[(int) type]);
            exit(1);
        }

        int nline;
        lex_line(lx, &nline);
        fprintf(stderr, "lex: line %d: expected token %d, but got %d\n", nline, names[(int) type], names[(int) lx->cur.type]);
        fprintf(stderr, "  %.*s\n", lx->cur.len, lx->cur.s);
        exit(1);
    }
    lex(lx);
}

#ifdef LEX_TEST
char *prog = 
    "fr($a, E) {\n"
    "  [$a] E\n"
    "       fr($a, E)\n"
    "}\n"
    ;

int main() {
    struct lex lx;
    lx.input = prog;
    lx.s = lx.input;
    do {
        lex(&lx);
        if (lx.cur.type == LT_ERROR) {
            int nline;
            lex_line(&lx, &nline);
            printf("lex: line %d: unexpected character: %c\n", nline, *(lx.s));
        } else if (lx.cur.type == LT_EOF) {
            printf("EOF\n");
        } else {
            printf("token(%s, %.*s)\n", names[(int) lx.cur.type], lx.cur.len, lx.cur.s);
        }
    } while (lx.cur.type != LT_EOF && lx.cur.type != LT_ERROR);
}
#endif
