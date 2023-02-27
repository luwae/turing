#include <iostream>
#include <cctype>
#include <stdexcept>
#include <map>

#include "tmlang/lex.hpp"

using std::ostream; using std::endl;
using std::map;
using std::string;
using std::runtime_error;

string Token::name(TokenType t) {
    static string names[] = {
        "eof", "error", "sym", "ident",
        "movel", "mover", "print", "lcurly", "rcurly",
        "lbracket", "rbracket",
        "comma",
        "def", "accept", "reject",
        "range"
    };
    return names[t];
}

string Token::substring() const {
    return _s->substr(_pos.off, _len);
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
    os << "Token(" << Token::name(t._type)
       << ", \"" << t.substring() << "\""
       << ", line=" << t._pos.lineno << ", lineoff=" << t._pos.lineoff
       << ")" << endl;
    return os;
}

ostream &Token::perror(ostream &os, const string &msg) const {

    if (_type == TokenType::eof) {
        os << "eof: " << msg << endl;
        return os;
    }

    os << _pos.lineno << ":" << (_pos.off - _pos.lineoff + 1) << ": " << msg << endl;
    os << " | ";
    for (size_t index = _pos.lineoff; (*_s)[index] != '\n' && (*_s)[index] != '\0'; index++)
        os << (*_s)[index];
    os << endl;
    os << " | ";
    for (size_t i = 0; i < _pos.off - _pos.lineoff; i++)
        os << " ";
    for (size_t i = 0; i < _len; i++)
        os << "~";
    os << endl;

    return os;
}

#define RANGE(c, l, h) ((c) >= (l) && (c) <= (h))
#define IS_IDENT1(c) (RANGE(c, 'A', 'Z') || RANGE(c, 'a', 'z') || (c) == '_')
#define IS_IDENT2(c) (IS_IDENT1(c) || RANGE(c, '0', '9'))
#define IS_HEX(c) (RANGE(c, '0', '9') || RANGE(c, 'a', 'f') || RANGE(c, 'A', 'F'))

char Lexer::peek() {
    if (_curr.off >= _s->size()) {
        _curr.off++;
        return '\0';
    }
    char c = (*_s)[_curr.off];
    if (c == '\n') {
        _curr.lineno++;
        _curr.lineoff = _curr.off + 1;
    }
    _curr.off++;
    return c;
}

void Lexer::back() {
    // cannot move before commit
    if (_curr.off == _comm.off)
        return;

    if (--_curr.off >= _s->size())
        return;
    char c = (*_s)[_curr.off];
    if (c != '\n')
        return;
    _curr.lineno--;
    _curr.lineoff = _comm.lineoff; // conservative estimate
    if (_curr.off == _comm.off)
        return; // avoid overflow lx->curr.off == 0
    for (size_t i = _curr.off - 1; i >= _comm.off; --i) {
        if ((*_s)[i] == '\n') {
            _curr.lineoff = i + 1;
            break;
        }
    }
}

bool Lexer::test_keyword(const std::string &keyword) {
    for (char c : keyword) {
        if (peek() != c) {
            revert();
            return false;
        }
    }
    char after = peek();
    if (IS_IDENT2(after)) { // guard against identifiers
        revert();
        return false;
    }
    commit_last();
    return true;
}

TokenType Lexer::handle_sym() {
    char c1 = peek();
    if (c1 == 'x') {
        char c2 = peek();
        if (IS_HEX(c2)) {
            char c3 = peek();
            if (IS_HEX(c3)) {
                char c4 = peek();
                if (c4 == '\'') {
                    commit();
                    _tok._type = TokenType::sym;
                    return TokenType::sym;
                }
            }
        }
    } else if (RANGE(c1, ' ', '~')) {
        char c2 = peek();
        if (c2 == '\'') {
            commit();
            _tok._type = TokenType::sym;
            return TokenType::sym;
        }
    }
    commit_last();
    return TokenType::error;
}

TokenType Lexer::handle_ident() {
    char c;
    do {
        c = peek();
    } while (IS_IDENT2(c));
    commit_last();
    _tok._type = TokenType::ident;
    return TokenType::ident;
}

TokenType Lexer::handle_string_ident() {
    char c;
    do {
        c = peek();
        if (c == '\"') {
            commit();
            _tok._type = TokenType::ident;
            return TokenType::ident;
        }
    } while (RANGE(c, ' ', '~'));
    commit_last();
    return TokenType::error;
}

// function left with '\0' or printable char except ' ' and '#'
// technically creates a "whitespace token", but we have no use for it
void Lexer::remove_junk() {
    char c;
    do {
        c = peek();
        if (c == '#') {
            do {
                c = peek();
                if (c == '\0') {
                    back();
                    break;
                }
            } while (c != '\n');
        } else if (c == '\0' || RANGE(c, '!', '~')) {
            commit_last();
            return;
        }
    } while (1);
}

map<char, TokenType> single_chars = {
    {'<', TokenType::movel}, {'>', TokenType::mover},
    {'{', TokenType::lcurly}, {'}', TokenType::rcurly},
    {'[', TokenType::lbracket}, {']', TokenType::rbracket},
    {'=', TokenType::print}, {',', TokenType::comma},
    {'-', TokenType::range}
};

map<string, TokenType> keywords = {
    {"def", TokenType::def}, {"accept", TokenType::accept}, {"reject", TokenType::reject}
};

TokenType Lexer::lex() {
    if (_tok._type == TokenType::eof || _tok._type == TokenType::error)
        return _tok._type;

    remove_junk();
    _tok.clear();
    
    char c = peek();
    if (c == '\0') {
        commit();
        _tok._type = TokenType::eof;
        return TokenType::eof;
    }
    
    auto found = single_chars.find(c);
    if (found != single_chars.end()) {
        commit();
        _tok._type = found->second;
        return found->second;
    }
    if (c == '\'')
        return handle_sym();
    if (c == '\"')
        return handle_string_ident();
    
    if (IS_IDENT1(c)) {
        revert();
        for (const auto &pair : keywords) {
            if (test_keyword(pair.first)) {
                _tok._type = pair.second;
                return pair.second;
            }
        }
        return handle_ident();
    }
    
    commit();
    return TokenType::error;
}
