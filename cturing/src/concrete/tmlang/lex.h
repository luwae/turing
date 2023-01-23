#ifndef LEX_H
#define LEX_H

#include "uchar_vec.h"

#define T_EOF 0
#define T_ERROR 1
#define T_CHR 2
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



typedef struct lexer Lexer;
typedef struct token Token;

struct token {
    int type;
    size_t len;
    size_t off;
    size_t line;
    size_t lineoff;
    Lexer *lx;
};

struct lexer {
    Token tok;
    struct {
        char c;
        size_t next;
        size_t line;
        size_t lineoff;
    } pos, pos_old;
    char *s;
    int done;
};




class Lexer;

class Token {
    friend class Lexer;
public:
    using size_type = std::string::size_type;

    Token()=default;
    Token (const Token &that)=default;
    Token &operator=(const Token &that)=default;

    static std::string name(TokenType t);
    std::string substring() const;
    std::string repr() const;
    std::ostream &perror(std::ostream &os, const std::string &msg) const;
    TokenType gettype() const { return type; }
private:
    TokenType type;
    size_type len;
    size_type offset;
    int line;
    size_type lineoff;
    const Lexer *lx;

    // only allow creation of tokens by lexer
    Token(TokenType type, size_type len, size_type offset, int line, size_type lineoff, const Lexer *lx):
        type(type), len(len), offset(offset), line(line), lineoff(lineoff), lx(lx) { }
};

class Lexer {
    friend class Token;
public:
    using size_type = std::string::size_type;
    Lexer(const std::string &input): s(input)
        { lex(); }
    const Token &gettok() { return tok; }
    void lex();
    void reset();
private:
    char getch();
    void ungetch();
    char newtoken();
    void remove_whitespace();
    struct {
        char c = '\0';
        size_type next = 0;
        int line = 1;
        size_type lineoff = 0;
    } pos, pos_old;
    const std::string s;
    bool done = false;
    Token tok;
};

}

#endif // LEX_HPP
