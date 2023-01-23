#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "lex.h"

#define BUFSIZE 1024 * 1024
char s[BUFSIZE];
size_t ptr;

static const char *Token_name(int type) {
    static char *names[] = {
        "eof", "error", "sym", "ident",
        "movel", "mover", "print", "lcurly", "rcurly",
        "lbracket", "rbracket",
        "comma",
        "def", "accept", "reject",
        "range"
    };
    return names[type];
}

static void Token_repr(const Token *tok) {
    if (tok->type == T_EOF)
        printf("Token(eof, line=%lu)\n", tok->lineno);
    else
        printf("Token(%s, %.*s, line=%lu)\n", Token_name(tok->type), (int) tok->len, tok->s + tok->off, tok->lineno);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: lex_test <.tm>\n");
        return -1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    int r;
    do {
        r = read(fd, s + ptr, BUFSIZE);
        ptr += r;
    } while (r > 0);
    if (r < 0) {
        perror("read");
        return -1;
    }
    close(fd);

    Lexer lx;
    Lexer_init(&lx, s);
    
    int status;
    do {
        status = lex(&lx);
        Token_repr(&lx.tok);
    } while (status != T_EOF && status != T_ERROR);
}
