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

    static std::string name(TokenType t);
    std::string substring() const;
    std::string repr() const;

    // TODO private attributes?

    TokenType type;
    size_type len;
    size_type offset;
    int line;
    size_type lineoff;
private:
    const Lexer *lx;
    Token()=default;
    Token(TokenType type, size_type len, size_type offset, int line, size_type lineoff, const Lexer *lx):
        type(type), len(len), offset(offset), line(line), lineoff(lineoff), lx(lx) { }
    Token &operator=(const Token &that) {
        type = that.type; len = that.len; offset = that.offset; line = that.line; lineoff = that.lineoff; lx = that.lx; return *this;
    }
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
    void expect(TokenType t, bool lex_after = true);
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
