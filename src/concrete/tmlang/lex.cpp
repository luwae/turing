#include <iostream>
#include <cctype>
#include <stdexcept>
#include <map>

#include "lex.hpp"

using std::ostream; using std::endl;
using std::map;
using std::string;
using std::runtime_error;

namespace lex {

string Token::name(TokenType t) {
    static string names[] = {
        "eof", "error", "chr", "ident",
        "movel", "mover", "print", "lcurly", "rcurly",
        "lbracket", "rbracket",
        "comma",
        "def", "accept", "reject",
        "range"
    };
    return names[t];
}

string Token::substring() const {
    if (type == TokenType::eof)
        return "";
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

int numlen(int num) {
    int len = 0;
    while (num) {
        num /= 10;
        len++;
    }
    return len;
}

ostream &Token::perror(ostream &os, const string &msg) const {

    if (type == TokenType::eof) {
        os << "eof: " << msg << endl;
        return os;
    }

    os << line << ":" << (offset - lineoff + 1) << ": " << msg << endl;
    os << " | ";
    for (size_type index = lineoff; lx->s[index] != '\n' && lx->s[index] != '\0'; index++)
        os << lx->s[index];
    os << endl;
    os << " | ";
    for (int i = 0; i < offset - lineoff; i++)
        os << " ";
    for (int i = 0; i < len; i++)
        os << "~";
    os << endl;

    return os;
}

bool isident1(char c) {
    return isalpha(c) || c == '_';
}

bool isident2(char c) {
    return isalpha(c) || c == '_' || (c >= '0' && c <= '9');
}

char Lexer::getch() {
    pos_old = pos;
    if (pos.c == '\n') {
        pos.line++;
        pos.lineoff = pos.next;
    }
    pos.c = s[pos.next++];
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

bool contains_keyword(const char *start, const string &keyword) {
    for (int i = 0; i < keyword.size(); i++) {
        if (start[i] != keyword[i])
            return false;
    }
    return !isident2(start[keyword.size()]); // guard against identifiers
}

void Lexer::lex() {
    _lex();
    std::cout << tok.repr() << std::endl;
}

void Lexer::remove_whitespace() {
    while (true) {
        while (isspace(getch()))
            ;
        ungetch();
        if (getch() == '#') {
            char c;
            do {
                c = getch();
            } while (c != '\n' && c != '\0');
            ungetch();
            if (c == '\0')
                return;
        } else {
            ungetch();
            return;
        }
    }
}

void Lexer::_lex() {
    if (done)
        return;

    remove_whitespace();

    static map<char, TokenType> single_chars = {
        {'<', movel}, {'>', mover},
        {'{', lcurly}, {'}', rcurly},
        {'[', lbracket}, {']', rbracket},
        {'=', print}, {',', comma},
        {'-', range}
    };

    static map<string, TokenType> keywords = {
        {"def", def}, {"accept", accept}, {"reject", reject}
    };

    char c = getch();
    tok = Token(error, 1, pos.next - 1, pos.line, pos.lineoff, this);
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
                tok.type = chr; tok.len = 1; tok.offset++;
                return;
            } else if (isxdigit(c2)) {
                char c3 = getch();
                if (isxdigit(c3)) {
                    char c4 = getch();
                    if (c4 == '\'') {
                        tok.type = chr; tok.len = 3; tok.offset++;
                        return;
                    }
                }
            }
        } else if (c1 >= ' ' && c1 <= '~') {
            char c2 = getch();
            if (c2 == '\'') {
                tok.type = chr; tok.len = 1; tok.offset++;
                return;
            }
        }
    } else if (isalpha(c) || c == '_') {
        
        for (auto it = keywords.begin(); it != keywords.end(); ++it) {
            if (contains_keyword(&s[tok.offset], it->first)) {
                for (int i = 1; i < it->first.size(); i++) // already getch() one
                    getch();
                tok.type = it->second; tok.len = it->first.size();
                return;
            }
        }

        while (isident2(getch()))
            ;
        ungetch();
        tok.type = ident; tok.len = pos.next - tok.offset;
        return;
    } else if (c == '"') {
    	do {
    	    c = getch();
    	} while (c != '"' && c != '\n' && c != '\0');
    	if (c == '"') { // valid identifier literal
    	    tok.type = ident; tok.len = pos.next - tok.offset - 2; tok.offset++;
    	    return;
    	}
    }

    // failed
    done = true;
}

}
