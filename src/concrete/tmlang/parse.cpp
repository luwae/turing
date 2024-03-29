#include <stdexcept>

#include "parse.hpp"

using std::string;
using std::runtime_error;
using std::ostream; using std::cout;

using lex::TokenType; using lex::Token; using lex::Lexer;

namespace parse {

string real_ident(const Token &t) {
    string s = t.substring();
    if (s[0] == '"') {
        s.erase(0, 1);
        s.erase(s.size() - 1, 1);
    }
    return s;
}

void expect(Lexer &lx, TokenType tt, bool lex_after = true) {
    if (lx.gettok().gettype() != tt) {
        lx.gettok().perror(cout, "expected " + Token::name(tt));
        throw runtime_error("fail");
    }
    if (lex_after)
        lx.lex();
}

void Parser::parse() {
    do {
        expect(lx, TokenType::ident, false);
        string name = real_ident(lx.gettok());
        auto found = statedescs.find(name);
        if (found != statedescs.end()) {
            lx.gettok().perror(cout, "duplicate state name");
            found->second.def.perror(cout, "--- previous definition ---");
            throw runtime_error("fail");
        }
        StateDesc desc;
        desc.index = states.size();
        desc.def = lx.gettok();
        statedescs[name] = desc;
        lx.lex();

        State s;
        s.name = name;
        expect(lx, TokenType::lcurly);
        parse_statebody(s);
        expect(lx, TokenType::rcurly);
        states.push_back(s); // collect states in parser because we need to resolve references later
    } while (lx.gettok().gettype() != TokenType::eof);

    // resolve states
    for (auto it = resolve.begin(); it != resolve.end(); ++it) {
        auto found = statedescs.find(real_ident(it->def));
        if (found == statedescs.end()) {
            it->def.perror(cout, "no fitting state definition");
            throw runtime_error("fail");
        }
        states[it->si].branches[it->bi].action.next = found->second.index;
    }
    
    // copy states to machine
    for (auto it = states.begin(); it != states.end(); ++it) {
        tm.add_state(*it);
    }
}

unsigned char convert_immediate(const string &s) {
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
            parse_nextstate(s, b.action);

            s.branches.push_back(b);
            return; // default is last branch
        } else if (lx.gettok().gettype() == TokenType::chr) {
            do {
            	unsigned char sym = convert_immediate(lx.gettok().substring());
            	b.syms.insert(sym);
            	lx.lex();
            	if (lx.gettok().gettype() == TokenType::range) {
            	   lx.lex();
            	   expect(lx, TokenType::chr, false);
            	   unsigned char stop = convert_immediate(lx.gettok().substring());
            	   if (stop <= sym) {
            	   	lx.gettok().perror(cout, "invalid range");
            	   	throw runtime_error("fail");
            	   }
            	   for (sym++; sym <= stop; sym++) // first symbol is already inserted
            	       b.syms.insert(sym);
            	   lx.lex();
            	}
            	if (lx.gettok().gettype() == TokenType::comma) {
            	    lx.lex();
            	    expect(lx, TokenType::chr, false);
            	} else {
            	    break;
            	}
            } while (true);
            
            expect(lx, TokenType::rbracket);
            parse_actions(b.action);
            parse_nextstate(s, b.action);
            // further branches may follow
        } else {
            lx.gettok().perror(cout, "branch specifier must be either 'def' or a list of symbols");
            throw runtime_error("fail");
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

void Parser::parse_nextstate(const State &s, Action &a) {
    if (lx.gettok().gettype() == TokenType::accept) {
        a.term = TerminateType::term_acc;
        lx.lex();
    } else if (lx.gettok().gettype() == TokenType::reject) {
        a.term = TerminateType::term_rej;
        lx.lex();
    } else if (lx.gettok().gettype() == TokenType::ident) {
        a.term = TerminateType::term_cont;
        resolve.emplace_back(states.size(), s.branches.size(), lx.gettok());
        lx.lex();
    } else if (lx.gettok().gettype() == TokenType::rcurly || lx.gettok().gettype() == TokenType::lbracket) { // default: stay
        a.term = TerminateType::term_cont;
        a.next = states.size(); // current state
    }
    // TODO else fail?
}

} // namespace parse
