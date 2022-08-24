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
        throw runtime_error("fail");
    }
    if (lex_after)
        lx.lex();
}

void Parser::parse() {
    parse_s();
    
    // TODO
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

void Parser::parse_s() {
    do {
        expect(lx, TokenType::ident, false);
        string name = lx.gettok().substring();
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
        
        parse_stateargs(s);
        
        expect(lx, TokenType::lcurly);
        parse_statebody(s);
        expect(lx, TokenType::rcurly);
        states.push_back(s); // collect states in parser because we need to resolve references later
    } while (lx.gettok().gettype() != TokenType::eof);
}

void Parser::parse_stateargs(State &s) {
    if (lx.gettok().gettype() == TokenType::lcurly)
        return;
    expect(lx, TokenType::lpar);
    parse_stateargs2(s);
    while (lx.gettok().gettype() == TokenType::comma) {
        lx.lex();
        parse_stateargs2(s);
    }
    expect(lx, TokenType::rpar);
}

void Parser::parse_stateargs2(State &s) {
    if (lx.gettok().gettype() == TokenType::ident) {
        s.args.emplace_back(StateArg::Type::sat_state_var, lx.gettok().substring());
        lx.lex();
    } else if (lx.gettok().gettype() == TokenType::varsym) {
        lx.lex();
        expect(lx, TokenType::ident, false);
        s.args.emplace_back(StateArg::Type::sat_sym_var, lx.gettok().substring());
        lx.lex();
    } else {
        lx.gettok().perror(cout, "expected state argument or symbol argument");
        throw runtime_error("fail");
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
    if (lx.gettok().gettype() == TokenType::rcurly)
        return;
    if (lx.gettok().gettype() != TokenType::lbracket) {
        lx.gettok().perror("expected branch");
        throw runtime_error("fail");
    }
    lx.lex();
    parse_statebody2(s);
}

void Parser::parse_statebody2(State &s) {
    if (lx.gettok().gettype() == TokenType::def) {
        Branch b;
        b.syms.emplace_back();
        parse_actions(s, b);
        parse_nextstate(s, b);
        s.branches.push_back(b);
    } else {
        Branch b;
        parse_sym(s);
        parse_symrange(s, b, parse_sym(s, b));
        parse_symargs(s, b);
        expect(lx, TokenType::rbracket);
        parse_actions(s, b);
        parse_nextstate(s, b);
        s.branches.push_back(b);
    }
}

int Parser::find_state_arg(State &s, StateArg::Type t, bool error = true) {
    for (int i = 0; i < s.args.size(); i++) {
        if (s.args[i].type == t && s.args[i].name == lx.gettok().substring()) {
            return i;
        }
    }
    if (error) {
        lx.gettok().perror("unknown arg");
        throw runtime_error("fail");
    }
    return -1;
}

Sym Parser::parse_sym(State &s) {
    if (lx.gettok().gettype() == TokenType::sym) {
        return Sym(false, convert_immediate(lx.gettok().substring())); 
    } else if (lx.gettok().gettype() == TokenType::varsym) {
    	lx.lex();
    	expect(lx, TokenType::ident, false);
    	return Sym(true, find_state_arg(s, StateArg::Type::sat_sym_var));
    }
}

void Parser::parse_symrange(State &s, Branch &b, Sym prev) {
    if (lx.gettok().gettype() == TokenType::range) {
    	lx.lex();
        Sym next = parse_sym(s);
        b.syms.emplace_back(prev, next);
    } else if (lx.gettok().gettype() == TokenType::rbracket || lx.gettok().gettype() == TokenType::comma) {
        b.syms.emplace_back(prev);
    } else {
        lx.gettok().perror("error parsing symrange");
    	throw runtime_error("fail");
    }
}

void Parser::parse_symargs(State &s, Branch &b) {
    while (lx.gettok().gettype() == TokenType::comma) {
        lx.lex();
        parse_symrange(s, b, parse_sym(s, b));
    }
}

void Parser::parse_actions(State &s, Branch &b) {
    while (true) {
        if (lx.gettok().gettype() == TokenType::movel) {
            b.action.primitives.emplace_back(Primitive::Type::pt_movel);
        } else if (lx.gettok().gettype() == TokenType::mover) {
            b.action.primitives.emplace_back(Primitive::Type::pt_mover);
        } else if (lx.gettok().gettype() == TokenType::print) {
            lx.lex();
            b.action.primitives.push_back(parse_sym(s));
        } else {
            break;
        }
    }
}

void Parser::parse_nextstate(State &s, Branch &b) {
    if (lx.gettok().gettype() == TokenType::lbracket || lx.gettok().gettype() == TokenType::rcurly) {
        return;
    }
    b.action.call = std::move(parse_call(s, b));
}

Call Parser::parse_call(State &s) {
    Call c;
    if (lx.gettok().gettype() == TokenType::accept) {
        c.type = ct_state_accept;
        return c;
    } else if (lx.gettok().gettype() == TokenType::reject) {
        c.type = ct_state_reject;
        return c;
    }
    expect(lx, TokenType::ident, false);
    string name = lx.gettok().substring();
    lx.lex();
    int index = find_state_arg(s, StateArg::type::sat_state_var, false);
    if (index >= 0) {
        c.type = ct_state_var;
        c.index = index;
        // arguments not possible
    } else {
        c.type = ct_state_imm;
        c.name = name;
        parse_callargs(s, c);
    }
    return c;
}

void Parser::parse_callargs(State &s, Call &c) {
    if (lx.gettok().gettype() == TokenType::lpar) {
        lx.lex();
        parse_callargs2(s, c);
        expect(lx, TokenType::rpar);
    }
}

void Parser::parse_callargs2(State &s, Call &c) {
    if (lx.gettok().gettype() == TokenType::sym || lx.gettok().gettype() == TokenType::varsym) {
        c.args.emplace_back(parse_sym(s));
    } else {
        c.args.emplace_back(std::move(parse_call(s)));
    }
    parse_callargs3(s, c);
}

void Parser::parse_callargs3(State &s, Call &c) {
    if (lx.gettok().gettype() == TokenType::rpar) {
        return;
    }
    expect(lx, TokenType::comma);
    parse_callargs2(s, c);
}

} // namespace parse
