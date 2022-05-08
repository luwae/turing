#include <stdexcept>

using std::string;
using std::runtime_error;

using lex::TokenType;

namespace parse {

void Parser::parseS() {
    while (lx.toktype() != TokenType::eof) {
        if (lx.toktype() != TokenType::ident) {
            throw runtime_error("expected new state name");
        }
        string name = lx.substring();
        lx.lex();
        if (states.find(name) != states.end()) {
            throw runtime_error("duplicate state name");
        }
        State *state = new State(name);
        states.insert({name, state})
        parseStateargs(state);
        lx.expect(TokenType::lcurly);
        parseStatebody(state);
        lx.expect(TokenType::rcurly);
    }
    
}

void Parser::parseStateargs(State *state) {
        if (lx.toktype() == TokenType::lcurly)
            return;
        lx.expect(lpar);
        while (true) {
            TokenType tt = lx.toktype();
            if (tt == TokenType::ident) {
                state->args.emplace_back(TokenType::state_var, lx.substring());
                lx.lex();
            } else if (tt == TokenType::chr_var) {
                lx.lex();
                if (lx.toktype() != TokenType::ident) {
                    throw runtime_error("expected char arg name");
                }
                state->args.emplace_back(TokenType::chr_var, lx.substring());
                lx.lex();
            } else {
                throw runtime_error("expected state arg or char arg, but got"
                        + lx.tokname(lx.toktype()));
            }

            tt = lx.toktype();
            if (tt == TokenType::rpar) {
                lx.lex();
                break;
            }
            lx.expect(TokenType::comma);
        }
}

void Parser::parseStatebody(State *state) {
    // TODO
}

}
