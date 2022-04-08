#include <stdio.h>

#include "lex.h"

#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\n' || (c) == '\t')
#define IS_ALPHANUM(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= '0' && (c) <= '9') || ((c) == '_'))

void lex_init(struct lex *lx, char *input) {
    lx->input = input;
    lx->s = input;
    lex(lx);
}

void lex(struct lex *lx) {
    char *s = lx->s;
    // remove whitespace + comments
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
    
    
    if (*s == '\0') {
        lx->cur.type = T_EOF;
    } else if (*s == '{' || *s == '}' || *s == '(' || *s == ')' || *s == '[' || *s == ']' || *s == ',') {
        lx->cur.type = *s++;
    } else if (IS_ALPHANUM(*s)) {
        lx->cur.type = T_IDENT;
        lx->cur.s = s;
        while (IS_ALPHANUM(*s)) s++;
        lx->cur.len = s - lx->cur.s;
        lx->s = s;
        return;
    } else {
        lx->cur.type = T_ERROR;
        lx->cur.s = s;
    }
    lx->s = s;
    return;
}

void error() {
    // TODO
}

void expect(struct lex *lx, char type) {
    if (lx->cur.type != type) error();
    lex(lx);
}

int main() {
    struct lex lx;
    lx.input = "  // test\nmain(E) {\n [0] r(fl(E))\n [1] l(/***/lfl(END))\n}\n // test";
    lx.s = lx.input;
    do {
        lex(&lx);
        if (lx.cur.type == T_IDENT) {
            printf("token(T_IDENT, %.*s)\n", lx.cur.len, lx.cur.s);
        } else if (lx.cur.type == T_EOF) {
            printf("token(T_EOF)\n");
        } else if (lx.cur.type == T_ERROR) {
            printf("token(T_ERROR)\n");
        } else {
            printf("token(%c)\n", lx.cur.type);
        }
    } while (lx.cur.type != T_EOF && lx.cur.type != T_ERROR);
}
