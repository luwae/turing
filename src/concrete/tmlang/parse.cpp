#include <stdexcept>

#include "parse.hpp"

using std::string;
using std::runtime_error;
using std::ostream;

using lex::TokenType; using lex::Token; using lex::Lexer;

namespace parse {

void Parser::parse() {
    do {
        lx.expect(TokenType::ident, false);
        string name = lx.gettok().substring();
        lx.lex();
        if (states.find(name) != states.end()) {
            throw runtime_error("duplicate state name: " + name);
        }
        states[name] = tm.size();

        State s;
        s.name = name;
        lx.expect(TokenType::lcurly);
        parse_statebody(s);
        lx.expect(TokenType::rcurly);
        tm.add_state(s);
    } while (lx.gettok().type != TokenType::eof);

    // resolve states
    for (int i = 0; i < resolve.size(); i++) {
        auto found = states.find(resolve[i].name);
        if (found == states.end()) {
            throw runtime_error("could not resolve state: " + resolve[i].name);
        }
        *(resolve[i].nextp) = found->second;
    }
}

unsigned char convert_immediate(const string &s) {

    // Note: immediates are of the form "'a'" (length 3) or "'\xf3'" (length 5)

    if (s.length() == 3)
        return s[1];

    unsigned char v = 0;
    if (s[2] <= '9')
        v += (s[2] - '0') << 4;
    else
        v += (s[2] - 'a' + 10) << 4;
    if (s[3] <= '9')
        v += s[3] - '0';
    else
        v += s[3] - 'a' + 10;
    return v;
}


void Parser::parse_statebody(State &s) {
    while (lx.gettok().type == TokenType::lbracket) { // new branch
        lx.lex();
        Branch b;
        if (lx.gettok().type == TokenType::def) {
            b.symset_invert = true;
            lx.lex();
            lx.expect(TokenType::rbracket);
            parse_actions(b.action);
            parse_nextstate(b.action);

            s.branches.push_back(b);
            return; // default is last branch
        } else if (lx.gettok().type == TokenType::chr) {
            b.syms.insert(convert_immediate(lx.gettok().substring()));
            lx.lex();
            while (lx.gettok().type == TokenType::comma) {
                lx.lex();
                lx.expect(TokenType::chr, false);
                b.syms.insert(convert_immediate(lx.gettok().substring()));
                lx.lex();
            }
            lx.expect(TokenType::rbracket);
            parse_actions(b.action);
            parse_nextstate(b.action);
            // further branches may follow
        }

        s.branches.push_back(b);
    }
}

void Parser::parse_actions(Action &a) {
    while (true) {
        if (lx.gettok().type == TokenType::movel) {
            a.primitives.emplace_back(PrimitiveType::pt_movel);
            lx.lex();
        } else if (lx.gettok().type == TokenType::mover) {
            a.primitives.emplace_back(PrimitiveType::pt_mover);
            lx.lex();
        } else if (lx.gettok().type == TokenType::print) {
            lx.lex();
            lx.expect(TokenType::chr, false);
            a.primitives.emplace_back(PrimitiveType::pt_print, convert_immediate(lx.gettok().substring()));
            lx.lex();
        } else {
            return; // found no more actions
        }
    }
}

void Parser::parse_nextstate(Action &a) {
    if (lx.gettok().type == TokenType::accept) {
        a.term = TerminateType::term_acc;
        lx.lex();
    } else if (lx.gettok().type == TokenType::reject) {
        a.term = TerminateType::term_rej;
        lx.lex();
    } else if (lx.gettok().type == TokenType::ident) {
        a.term = TerminateType::term_cont;
        resolve.emplace_back(&a.next, lx.gettok().substring());
        lx.lex();
    } else if (lx.gettok().type == TokenType::rcurly || lx.gettok().type == TokenType::lbracket) { // default: stay
        a.term = TerminateType::term_cont;
        a.next = tm.size(); // current state
    }
    // TODO else fail?
}

} // namespace parse
