#include <cctype>
#include <stdexcept>
#include <map>

#include "lex.hpp"

using std::map;
using std::string;
using std::runtime_error;

namespace lex {

string Lexer::substring() const {
    if (type == eof) {
        return "";
    }
    return s.substr(offset, len);
}

void Lexer::lex() {
    if (done)
        return;

    while(isspace(s[cur]))
        ++cur;

    static map<char, TokenType> single_chars = {
        {'<', movel}, {'>', mover},
        {'{', lcurly}, {'}', rcurly},
        {'(', lpar}, {')', rpar},
        {'[', lbracket}, {']', rbracket},
        {'=', print}, {',', comma}, {'$', chr_var}
    };

    char c = s[cur];
    auto single_found = single_chars.find(c);
    if (c == '\0') {
        done = true;
        settoken(eof, 1, cur);
        return;
    } else if (single_found != single_chars.end()) {
        settoken(single_found->second, 1, cur);
        ++cur;
        return;
    } else if (c == '\'') {
        char c1 = s[cur + 1];
        if (c1 == 'x') {
            char c2 = s[cur + 2];
            if (c2 == '\'') {
                settoken(chr_imm, 1, cur + 1);
                cur += 3;
                return;
            } else if (isxdigit(c2)) {
                char c3 = s[cur + 3];
                if (isxdigit(c3)) {
                    char c4 = s[cur + 4];
                    if (c4 == '\'') {
                        settoken(chr_imm, 3, cur + 1);
                        cur += 5;
                        return;
                    }
                }
            }
        } else if (c1 >= ' ' && c1 <= '~') {
            char c2 = s[cur + 2];
            if (c2 == '\'') {
                settoken(chr_imm, 1, cur + 1);
                cur += 3;
                return;
            }
        }
    } else if (isalpha(c) || c == '_') {
        auto start = cur;
        ++cur;
        while (isalnum(s[cur]) || s[cur] == '_')
            ++cur;
        settoken(ident, cur - start, start);
        return;
    }

    done = true;
    settoken(error, 1, cur);
}

string Lexer::tokname(TokenType t) {
    static string names[] = {
        "eof", "error", "chr_imm", "ident",
        "movel", "mover", "print", "lcurly", "rcurly",
        "lpar", "rpar", "lbracket", "rbracket",
        "comma", "chr_var"
    };
    return names[t];
}

void Lexer::expect(TokenType t) {
    if (type != t)
        throw runtime_error("expected " + tokname(t) + " but got " + tokname(type));
    lex();
}

}
