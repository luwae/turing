#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define T_EOF 0
#define T_FAIL 1
#define T_NAME 2
#define T_STATE 3
#define T_SYMDEF 4
#define T_MATCHDEF 5
#define T_WILDDEF 6
#define T_SYM 7
#define T_LCURLY 8
#define T_RCURLY 9
#define T_LMOVE 10
#define T_RMOVE 11

struct token {
    char type;
    int len;
    char *start;
};

char *in;

int alphanum(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    return c == '_';
}

int hex(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    return (c >= 'a' && c <= 'f');
}

int whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t'; 
}

int inv(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\0';
}


void lex(struct token *t) {
    while (whitespace(*in)) {
        in++;
    }

    switch (*in) {
    case '\0':
        t->type = T_EOF;
        return;
    case '[':
        t->type = T_STATE;
        t->start = ++in;
        if (alphanum(*in)) {
            in++;
            while (alphanum(*in)) {
                in++;
            }
            if (*in == '.') { // reference foreign machine
                in++;
                if (!alphanum(*in)) {
                    t->type = T_FAIL;
                    return;
                }
                in++;
                while (alphanum(*in)) {
                    in++;
                }
            }
        }
        if (*in != ']') {
            t->type = T_FAIL;
            return;
        }
        t->len = (int) (in++ - t->start);
        return;
    case '{':
        t->type = T_LCURLY;
        in++;
        return;
    case '}':
        t->type = T_RCURLY;
        in++;
        return;
    case '<':
        t->type = T_LMOVE;
        in++;
        return;
    case '>':
        t->type = T_RMOVE;
        in++;
        return;
    case '|':
        t->type = T_SYMDEF;
        t->start = ++in;
        if (inv(*in)) { // read any char
            t->type = T_FAIL;
            return;
        } else if (*in == 'x') {
            if (!hex(*(in + 1)) || !hex(*(in + 2))) {
                t->type = T_FAIL;
                return;
            }
            in += 3;
            t->len = 3;
            return;
        }
        in++;
        t->len = 1;
        return;
    case '$':
        t->type = T_SYM;
        t->start = ++in;
        if (inv(*in)) { // read any char
            t->type = T_FAIL;
            return;
        } else if (*in == 'x') {
            if (!hex(*(in + 1)) || !hex(*(in + 2))) {
                t->type = T_FAIL;
                return;
            }
            in += 3;
            t->len = 3;
            return;
        }
        in++;
        t->len = 1;
        return;
    case '?':
        t->type = T_MATCHDEF;
        t->start = ++in;
        if (inv(*in) || inv(*(in + 1))) {
            t->type = T_FAIL;
            return;
        }
        in += 2;
        while (!inv(*in)) {
            in++;
        }
        if (!whitespace(*in)) {
            t->type = T_FAIL;
            return;
        }
        t->len = (int) (in++ - t->start);
        return;
    case '@':
        t->type = T_WILDDEF;
        in++;
        return;
    }
    
    if (alphanum(*in)) {
        t->type = T_NAME;
        t->start = in++;
        while (alphanum(*in)) {
            in++;
        }
        t->len = (int) (in - t->start);
        return;
    }
    
    t->type = T_FAIL;
    return;
}

char buf[4096];

char *names[] = {
    "EOF",
    "FAIL",
    "NAME",
    "STATE",
    "SYMDEF",
    "MATCHDEF",
    "WILDDEF",
    "SYM",
    "LCURLY",
    "RCURLY",
    "LMOVE",
    "RMOVE"
};

int varlen[] = {
    0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0
};

int main() {
    int fd = open("cursed.tm", O_RDONLY);
    if (fd == -1)
        return -1;
    int r = read(fd, buf, 4096);
    if (r == -1) {
        close(fd);
        return -1;
    }
    buf[r] = '\0';
    printf("read %d bytes\n", r);
    in = buf;
    
    struct token t;
    do {
        lex(&t);
        if (varlen[t.type])
            printf("Token(%s, '%.*s')\n", names[t.type], t.len, t.start);
        else
            printf("Token(%s)\n", names[t.type]);
    } while (t.type != T_EOF && t.type != T_FAIL);
    
    close(fd);
}
