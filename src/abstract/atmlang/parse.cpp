#include <stdexcept>

#include "parse.hpp"

using std::string;
using std::set;
using std::runtime_error;
using std::ostream; using std::cout; using std::endl;

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
    
    for (const auto &s: states)
        cout << s << "\n";
    cout << "\n\n\n" << endl;

    if (states[0].args.size() > 0) {
        throw runtime_error("main state is not allowed to have arguments");
    }
    
    State maincopy = states[0];
    set<string> expandednames;
    maincopy.expand(cout, states, expandednames);
}

void Parser::parse_s() {
    cout << "parse_s" << endl;
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
    cout << "parse_stateargs" << endl;
    if (lx.gettok().gettype() == TokenType::lcurly) {
        return;
    } else if (lx.gettok().gettype() == TokenType::lpar) {
        lx.lex();
        parse_stateargs2(s);
        while (lx.gettok().gettype() == TokenType::comma) {
            lx.lex();
            parse_stateargs2(s);
        }
        expect(lx, TokenType::rpar);
    } else {
        lx.gettok().perror(cout, "expected state args or state body");
        throw runtime_error("fail");
    }
}

void Parser::parse_stateargs2(State &s) {
    cout << "parse_stateargs2" << endl;
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
    cout << "parse_statebody" << endl;
    if (lx.gettok().gettype() == TokenType::rcurly) {
        return;
    }
    if (lx.gettok().gettype() != TokenType::lbracket) {
        lx.gettok().perror(cout, "expected branch");
        throw runtime_error("fail");
    }
    lx.lex();
    parse_statebody2(s);
}

void Parser::parse_statebody2(State &s) {
    cout << "parse_statebody2" << endl;
    if (lx.gettok().gettype() == TokenType::def) {
        lx.lex();
        Branch b;
        b.syms.emplace_back();
        expect(lx, TokenType::rbracket);
        parse_actions(s, b);
        parse_nextstate(s, b);
        s.branches.push_back(b);
    } else {
        Branch b;
        parse_symrange(s, b, parse_sym(s));
        parse_symargs(s, b);
        expect(lx, TokenType::rbracket);
        parse_actions(s, b);
        parse_nextstate(s, b);
        s.branches.push_back(b);
        // repeat
        parse_statebody(s);
    }
}

int Parser::find_state_arg(State &s, StateArg::Type t, bool error) {
    for (unsigned int i = 0; i < s.args.size(); i++) {
        if (s.args[i].type == t && s.args[i].name == lx.gettok().substring()) {
            return i;
        }
    }
    if (error) {
        lx.gettok().perror(cout, "unknown arg");
        throw runtime_error("fail");
    }
    return -1;
}

Sym Parser::parse_sym(State &s) {
    cout << "parse_sym" << endl;
    if (lx.gettok().gettype() == TokenType::sym) {
        Sym sy(false, convert_immediate(lx.gettok().substring())); 
        lx.lex();
        return sy;
    } else if (lx.gettok().gettype() == TokenType::varsym) {
    	lx.lex();
    	expect(lx, TokenType::ident, false);
    	Sym sy(true, find_state_arg(s, StateArg::Type::sat_sym_var));
    	lx.lex();
    	return sy;
    }
    lx.gettok().perror(cout, "expected symbol");
    throw runtime_error("fail");
}

void Parser::parse_symrange(State &s, Branch &b, Sym prev) {
    cout << "parse_symrange" << endl;
    if (lx.gettok().gettype() == TokenType::range) {
    	lx.lex();
        Sym next = parse_sym(s);
        b.syms.emplace_back(prev, next);
    } else if (lx.gettok().gettype() == TokenType::rbracket || lx.gettok().gettype() == TokenType::comma) {
        b.syms.emplace_back(prev);
    } else {
        lx.gettok().perror(cout, "error parsing symrange");
    	throw runtime_error("fail");
    }
}

void Parser::parse_symargs(State &s, Branch &b) {
    cout << "parse_symargs" << endl;
    while (lx.gettok().gettype() == TokenType::comma) {
        lx.lex();
        parse_symrange(s, b, parse_sym(s));
    }
}

void Parser::parse_actions(State &s, Branch &b) {
    cout << "parse_actions" << endl;
    while (true) {
        if (lx.gettok().gettype() == TokenType::movel) {
            lx.lex();
            b.action.primitives.emplace_back(Primitive::Type::pt_movel);
        } else if (lx.gettok().gettype() == TokenType::mover) {
            lx.lex();
            b.action.primitives.emplace_back(Primitive::Type::pt_mover);
        } else if (lx.gettok().gettype() == TokenType::print) {
            lx.lex();
            b.action.primitives.emplace_back(parse_sym(s));
        } else {
            break;
        }
    }
}

void Parser::parse_nextstate(State &s, Branch &b) {
    cout << "parse_nextstate" << endl;
    if (lx.gettok().gettype() == TokenType::lbracket || lx.gettok().gettype() == TokenType::rcurly) {
        return;
    }
    b.action.call = std::move(parse_call(s));
}

std::unique_ptr<Call> Parser::parse_call(State &s) {
    cout << "parse_call" << endl;
    std::unique_ptr<Call> c = std::make_unique<Call>();
    if (lx.gettok().gettype() == TokenType::accept) {
        lx.lex();
        c->type = Call::Type::ct_state_accept;
        return c;
    } else if (lx.gettok().gettype() == TokenType::reject) {
        lx.lex();
        c->type = Call::Type::ct_state_reject;
        return c;
    }
    expect(lx, TokenType::ident, false);
    string name = lx.gettok().substring();
    int index = find_state_arg(s, StateArg::Type::sat_state_var, false);
    lx.lex();
    if (index >= 0) {
        c->type = Call::Type::ct_state_var;
        c->index = index;
        // arguments not possible
    } else {
        c->type = Call::Type::ct_state_imm;
        c->name = name;
        c = parse_callargs(s, std::move(c));
    }
    return c;
}

std::unique_ptr<Call> Parser::parse_callargs(State &s, std::unique_ptr<Call> c) {
    cout << "parse_callargs" << endl;
    if (lx.gettok().gettype() == TokenType::lpar) {
        lx.lex();
        c = parse_callargs2(s, std::move(c));
        expect(lx, TokenType::rpar);
    }
    return c;
}

std::unique_ptr<Call> Parser::parse_callargs2(State &s, std::unique_ptr<Call> c) {
    cout << "parse_callargs2" << endl;
    if (lx.gettok().gettype() == TokenType::sym || lx.gettok().gettype() == TokenType::varsym) {
        c->args.emplace_back(parse_sym(s));
    } else {
        c->args.emplace_back(parse_call(s));
    }
    return parse_callargs3(s, std::move(c));
}

std::unique_ptr<Call> Parser::parse_callargs3(State &s, std::unique_ptr<Call> c) {
    cout << "parse_callargs3" << endl;
    if (lx.gettok().gettype() == TokenType::rpar) {
        return c;
    }
    expect(lx, TokenType::comma);
    return parse_callargs2(s, std::move(c));
}

} // namespace parse
