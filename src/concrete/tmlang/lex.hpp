#ifndef LEX_HPP
#define LEX_HPP

#include <string>

namespace lex {

enum TokenType {
    eof = 0,
    error = 1,
    chr = 2,
    ident = 3,

    movel = 4,
    mover = 5,
    print = 6,
    lcurly = 7,
    rcurly = 8,
    lbracket = 9,
    rbracket = 10,
    comma = 11,

    def = 12,
    accept = 13,
    reject = 14,
};

class Lexer;

class Token {
    friend class Lexer;
public:
    using size_type = std::string::size_type;

    Token()=default;
    Token (const Token &that): type(that.type), len(that.len), offset(that.offset), line(that.line), lineoff(that.lineoff), lx(that.lx) {}
    Token &operator=(const Token &that) {
        type = that.type; len = that.len; offset = that.offset; line = that.line; lineoff = that.lineoff; lx = that.lx; return *this;
    }

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
    void _lex();
    void reset();
private:
    char getch();
    void ungetch();
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
