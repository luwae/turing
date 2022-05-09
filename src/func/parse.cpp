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
    if (lx.toktype() == TokenType::rcurly)
        return;
    TokenType tt = lx.toktype();
    if (tt == TokenType::movel || tt == TokenType::mover
            || tt == TokenType::print || tt == TokenType::ident) {
        parseActions(state->deflt);
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

int find_arg(const state *s, const string &substring, StateArgType t) {
    int index = 0;
    for (const auto &arg : s->args) {
        if (arg.name == substring && arg.type == t)
            break;
        ++index;
    }
    return index;
}

CallArg Parser::parseChar(const State *s) {
    TokenType tt = lx.toktype();
    if (tt == TokenType::chr_imm) {
        string name = lx.substring();
        lx.lex();
        return CallArg(CallArgType::chr_imm, convert_immediate(name));
    } else if (tt == TokenType::chr_var) {
        string name = lx.substring()
        int index = find_arg(s, name, StateArgType::chr_var);
        if (index == s->args.size())
            throw runtime_error("unrecognized char argument " + name);
        lx.lex();
        return CallArg(CallArgType::chr_var, index);
    } else {
        throw runtime_error("expected character");
    }
}

void Parser::parseActions(State *state, Action &action) {
    TokenType tt;

    while ((tt = lx.toktype()) != TokenType::ident) {
        if (tt == TokenType::movel) {
            action.primitives.emplace_back(PrimitiveType::movel);
            lx.lex();
        } else if (tt == TokenType::mover) {
            action.primitives.emplace_back(PrimitiveType::mover);
            lx.lex();
        } else if (tt == TokenType::print) {
            lx.lex();
            tt = lx.toktype();
            if (tt == TokenType::chr_imm) {
                action.primitives.emplace_back(PrimitiveType::print,
                        convert_immediate(lx.substring()));
                lx.lex();
            } else if (tt == TokenType::chr_var) {
                int index = 0;
                string s = lx.substring();
                for (const auto &arg : state->args) {
                    if (arg.name == s)
                        break;
                    index++;
                }
                if (index == state->args.size())
                    throw runtime_error("unrecognized char argument");
                action.primitives.emplace_back(PrimitiveType::print, index);
                lx.lex();
            } else {
                throw runtime_error("expected char to print");
            }
        } else {
            throw runtime_error("expected primitive action (move or print)");
        }
    }
}

Parser::parseCharargs(State *s, Branch *b)
    while (true) {
        b->chars.push_back(parseChar(s));
        if (lx.toktype() != TokenType::comma)
            break;
        lx.lex();
    }
    lx.expect(TokenType::rbracket);
}
