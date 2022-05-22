#include <stdexcept>

#include "parse.hpp"

using std::string;
using std::runtime_error;
using std::ostream;

using lex::TokenType;

namespace parse {

void Parser::parse() {
    while (lx.toktype() != TokenType::eof) {
        if (lx.toktype() != TokenType::ident) {
            throw runtime_error("expected state name");
        }
        string name = lx.substring();
        lx.lex();
        if (tm.contains_state(name))
                throw runtime_error("duplicate state name");
        State s;
        s.name = name;
        lx.expect(TokenType::lcurly);
        parseStatebody(s);
        lx.expect(TokenType::rcurly);
        tm.add_state(s);
    }
    
}

void Parser::parseStatebody(State &s) {
    if (lx.toktype() == TokenType::rcurly)
        return;

    while (lx.toktype() == TokenType::lbracket) {
        lx.lex();
        Branch b;
        parseCharargs(s, b);
        lx.expect(TokenType::rbracket);

        parsePrimitives(s, b.action);
        if (lx.toktype() != TokenType::ident)
            throw runtime_error("expected state transition");
        b.action.next = lx.substring();
        lx.lex();
        s.branches.push_back(b);
    }

    TokenType tt = lx.toktype();
    if (tt == TokenType::movel || tt == TokenType::mover
            || tt == TokenType::print || tt == TokenType::ident) {
        parsePrimitives(s, s.deflt);
        if (lx.toktype() != TokenType::ident)
            throw runtime_error("expected state transition default");
        s.deflt.next = lx.substring();
        lx.lex();
    }
}

unsigned char convert_immediate(const string &s) {
    if (s.length() == 1)
        return s[0];

    unsigned char v = 0;
    if (s[1] <= '9')
        v += (s[1] - '0') << 4;
    else
        v += (s[1] - 'a' + 10) << 4;
    if (s[2] <= '9')
        v += s[2] - '0';
    else
        v += s[2] - 'a' + 10;
    return v;
}

void Parser::parsePrimitives(State &s, Action &a) {
    TokenType tt;

    while (lx.toktype() == TokenType::movel || lx.toktype() == TokenType::mover ||
            lx.toktype() == TokenType::print) {
        tt = lx.toktype();
        if (tt == TokenType::movel) {
            a.primitives.emplace_back(PrimitiveType::pt_movel, 0);
            lx.lex();
        } else if (tt == TokenType::mover) {
            a.primitives.emplace_back(PrimitiveType::pt_mover, 0);
            lx.lex();
        } else {
            lx.lex();
            tt = lx.toktype();
            if (tt != TokenType::chr)
                throw runtime_error("expected char");
            a.primitives.emplace_back(PrimitiveType::pt_print, convert_immediate(lx.substring()));
            lx.lex();
        }
    }
}

void Parser::parseCharargs(State &s, Branch &b) {
    while (true) {
        if (lx.toktype() != TokenType::chr)
            throw runtime_error("expected char arg");
        b.chars.insert(convert_immediate(lx.substring()));
        lx.lex();
        if (lx.toktype() != TokenType::comma)
            break;
        lx.lex();
    }
}

} // namespace parse
