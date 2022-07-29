#include <stdexcept>

#include "parse.hpp"

using std::string;
using std::runtime_error;
using std::ostream;

using lex::TokenType; using lex::Token; using lex::Lexer;

namespace parse {

void Parser::parse() {
    while (lx.gettok().type != TokenType::eof) {
        if (lx.gettok().type != TokenType::ident) {
            throw runtime_error("expected state name");
        }
        string name = lx.gettok().substring();
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
    if (lx.gettok().type == TokenType::rcurly)
        return;

    while (lx.gettok().type == TokenType::lbracket) {
        lx.lex();
        Branch b;
        parseCharargs(s, b);
        lx.expect(TokenType::rbracket);

        parsePrimitives(s, b.action);
        if (lx.gettok().type != TokenType::ident)
            throw runtime_error("expected state transition");
        b.action.next = lx.gettok().substring();
        lx.lex();
        s.branches.push_back(b);
    }

    TokenType tt = lx.gettok().type;
    if (tt == TokenType::movel || tt == TokenType::mover
            || tt == TokenType::print || tt == TokenType::ident) {
        parsePrimitives(s, s.deflt);
        if (lx.gettok().type != TokenType::ident)
            throw runtime_error("expected state transition default");
        s.deflt.next = lx.gettok().substring();
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

    while (lx.gettok().type == TokenType::movel || lx.gettok().type == TokenType::mover ||
            lx.gettok().type == TokenType::print) {
        tt = lx.gettok().type;
        if (tt == TokenType::movel) {
            a.primitives.emplace_back(PrimitiveType::pt_movel, 0);
            lx.lex();
        } else if (tt == TokenType::mover) {
            a.primitives.emplace_back(PrimitiveType::pt_mover, 0);
            lx.lex();
        } else {
            lx.lex();
            tt = lx.gettok().type;
            if (tt != TokenType::chr)
                throw runtime_error("expected char");
            a.primitives.emplace_back(PrimitiveType::pt_print, convert_immediate(lx.gettok().substring()));
            lx.lex();
        }
    }
}

void Parser::parseCharargs(State &s, Branch &b) {
    while (true) {
        if (lx.gettok().type != TokenType::chr)
            throw runtime_error("expected char arg");
        b.chars.insert(convert_immediate(lx.gettok().substring()));
        lx.lex();
        if (lx.gettok().type != TokenType::comma)
            break;
        lx.lex();
    }
}

} // namespace parse
