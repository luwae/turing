#include <cctype>
#include <stdexcept>

#include "lex.hpp"

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

    string single_chars("<>={}()[],$");
    char c = s[cur];
    if (c == '\0') {
        done = true;
        settoken(eof, 1, cur);
        return;
    } else if (single_chars.find(c) != string::npos) {
        settoken((TokenType) c, 1, cur);
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
        } else if (c1 >= '!' && c1 <= '~') {
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
    };
    if (t <= 3) {
        return names[t];
    }
    return string(1, t);
}

void Lexer::expect(TokenType t) {
    if (type != t)
        throw runtime_error("expected " + tokname(t) + " but got " + tokname(type));
    lex();
}

}
