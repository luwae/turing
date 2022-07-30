#include <stdexcept>

#include "parse.hpp"

using std::string;
using std::runtime_error;
using std::ostream; using std::cout;

using lex::TokenType; using lex::Token; using lex::Lexer;

namespace parse {

void expect(Lexer &lx, TokenType tt, bool lex_after = true) {
    if (lx.gettok().gettype() != tt) {
        lx.gettok().perror(cout, "expected " + Token::name(tt));
        throw runtime_error("a");
    }
    if (lex_after)
        lx.lex();
}

void Parser::parse() {
    do {
        expect(lx, TokenType::ident, false);
        string name = lx.gettok().substring();
        auto found = states.find(name);
        if (found != states.end()) {
            lx.gettok().perror(cout, "duplicate state name");
            found->second.def.perror(cout, "--- previous definition ---");
            throw runtime_error("a");
        }
        StateDesc desc;
        desc.index = tm.size();
        desc.def = lx.gettok();
        states[name] = desc;
        lx.lex();

        State s;
        s.name = name;
        expect(lx, TokenType::lcurly);
        parse_statebody(s);
        expect(lx, TokenType::rcurly);
        tm.add_state(s);
    } while (lx.gettok().gettype() != TokenType::eof);

    // TODO save resolve tokens to refer to them on error.

    // resolve states
    for (int i = 0; i < resolve.size(); i++) {
        auto found = states.find(resolve[i].name);
        if (found == states.end()) {
            resolve[i].def.perror(cout, "no fitting state definition");
            throw runtime_error("a");
        }
        *(resolve[i].nextp) = found->second.index;
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
    while (lx.gettok().gettype() == TokenType::lbracket) { // new branch
        lx.lex();
        Branch b;
        if (lx.gettok().gettype() == TokenType::def) {
            b.symset_invert = true;
            lx.lex();
            expect(lx, TokenType::rbracket);
            parse_actions(b.action);
            parse_nextstate(b.action);

            s.branches.push_back(b);
            return; // default is last branch
        } else if (lx.gettok().gettype() == TokenType::chr) {
            b.syms.insert(convert_immediate(lx.gettok().substring()));
            lx.lex();
            while (lx.gettok().gettype() == TokenType::comma) {
                lx.lex();
                expect(lx, TokenType::chr, false);
                b.syms.insert(convert_immediate(lx.gettok().substring()));
                lx.lex();
            }
            expect(lx, TokenType::rbracket);
            parse_actions(b.action);
            parse_nextstate(b.action);
            // further branches may follow
        } else {
            lx.gettok().perror(cout, "branch specifier must be either 'def' or a list of symbols");
            throw runtime_error("a");
        }

        s.branches.push_back(b);
    }
}

void Parser::parse_actions(Action &a) {
    while (true) {
        if (lx.gettok().gettype() == TokenType::movel) {
            a.primitives.emplace_back(PrimitiveType::pt_movel);
            lx.lex();
        } else if (lx.gettok().gettype() == TokenType::mover) {
            a.primitives.emplace_back(PrimitiveType::pt_mover);
            lx.lex();
        } else if (lx.gettok().gettype() == TokenType::print) {
            lx.lex();
            expect(lx, TokenType::chr, false);
            a.primitives.emplace_back(PrimitiveType::pt_print, convert_immediate(lx.gettok().substring()));
            lx.lex();
        } else {
            return; // found no more actions
        }
    }
}

void Parser::parse_nextstate(Action &a) {
    if (lx.gettok().gettype() == TokenType::accept) {
        a.term = TerminateType::term_acc;
        lx.lex();
    } else if (lx.gettok().gettype() == TokenType::reject) {
        a.term = TerminateType::term_rej;
        lx.lex();
    } else if (lx.gettok().gettype() == TokenType::ident) {
        a.term = TerminateType::term_cont;
        resolve.emplace_back(&a.next, lx.gettok().substring(), lx.gettok());
        lx.lex();
    } else if (lx.gettok().gettype() == TokenType::rcurly || lx.gettok().gettype() == TokenType::lbracket) { // default: stay
        a.term = TerminateType::term_cont;
        a.next = tm.size(); // current state
    }
    // TODO else fail?
}

} // namespace parse
