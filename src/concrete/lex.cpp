#include <iostream>
#include <cctype>
#include <stdexcept>
#include <map>

#include "lex.hpp"

using std::map;
using std::string;
using std::runtime_error;

namespace lex {

string Token::name(TokenType t) {
    static string names[] = {
        "eof", "error", "chr", "ident",
        "movel", "mover", "print", "lcurly", "rcurly",
        "lbracket", "rbracket",
        "comma"
    };
    return names[t];
}

string Token::substring() const {
    return lx->s.substr(offset, len);
}

string Token::repr() const {
    return "Token(type=" + name(type)
        + ", len=" + std::to_string(len)
        + ", offset=" + std::to_string(offset)
        + ", line=" + std::to_string(line)
        + ", lineoff=" + std::to_string(lineoff)
        + ", str=\"" + substring()  + "\")";
}

bool isident2(char c) {
    // allow more characters than would usually be the case, for conversion
    string special = "()',";
    return isalpha(c) || c == '_' || (c >= '0' && c <= '9') || special.find(c) != string::npos;
}

char Lexer::getch() {
    pos_old = pos;
    pos.c = s[pos.next++];
    if (pos.c == '\n') {
        pos.line++;
        pos.lineoff = pos.next;
    }
    return pos.c;
}

void Lexer::ungetch() {
    pos = pos_old;
}

void Lexer::reset() {
    pos = pos_old = {'\0', 0, 1, 0};
    done = false;
    lex();
}

void Lexer::lex() {
    if (done)
        return;

    while (isspace(getch()))
        ;
    ungetch();

    static map<char, TokenType> single_chars = {
        {'<', movel}, {'>', mover},
        {'{', lcurly}, {'}', rcurly},
        {'[', lbracket}, {']', rbracket},
        {'=', print}, {',', comma}
    };

    tok = Token(error, 1, pos.next, pos.line, pos.lineoff, this);
    char c = getch();
    auto single_found = single_chars.find(c);
    if (c == '\0') {
        done = true;
        tok.type = eof;
        return;
    } else if (single_found != single_chars.end()) {
        tok.type = single_found->second;
        return;
    } else if (c == '\'') {
        char c1 = getch();
        if (c1 == 'x') {
            char c2 = getch();
            if (c2 == '\'') {
                tok.type = chr; tok.len = 3;
                return;
            } else if (isxdigit(c2)) {
                char c3 = getch();
                if (isxdigit(c3)) {
                    char c4 = getch();
                    if (c4 == '\'') {
                        tok.type = chr; tok.len = 5;
                        return;
                    }
                }
            }
        } else if (c1 >= ' ' && c1 <= '~') {
            char c2 = getch();
            if (c2 == '\'') {
                tok.type = chr; tok.len = 3;
                return;
            }
        }
    } else if (isalpha(c) || c == '_') {
        while (isident2(getch()))
            ;
        ungetch();
        tok.type = ident; tok.len = pos.next - tok.offset;
        return;
    }

    // failed
    done = true;
}


void Lexer::expect(TokenType t) {
    if (tok.type != t)
        throw runtime_error("expected " + Token::name(t) + " but got " + tok.repr());
    lex();
}

}
