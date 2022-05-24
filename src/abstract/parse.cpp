#include <stdexcept>

#include "parse.hpp"

using std::string;
using std::runtime_error;
using std::ostream;
using std::unique_ptr; using std::make_unique;

using lex::TokenType;

namespace parse {

void Parser::parse() {
    while (lx.toktype() != TokenType::eof) {
        if (lx.toktype() != TokenType::ident) {
            throw runtime_error("expected state name");
        }
        string name = lx.substring();
        lx.lex();
        for (const auto &s : states)
            if (s.name == name)
                throw runtime_error("duplicate state name");
        states.emplace_back();
        State &s = states.back();
        s.name = name;
        parseStateargs(s);
        lx.expect(TokenType::lcurly);
        parseStatebody(s);
        lx.expect(TokenType::rcurly);
    }
}

void Parser::parseStateargs(State &state) {
    if (lx.toktype() == TokenType::lcurly)
        return;
    lx.expect(TokenType::lpar);
    while (true) {
        TokenType tt = lx.toktype();
        if (tt == TokenType::ident) {
            state.args.emplace_back(StateArg::Type::sat_state_var, lx.substring());
            lx.lex();
        } else if (tt == TokenType::chr_var) {
            lx.lex();
            if (lx.toktype() != TokenType::ident) {
               throw runtime_error("expected char arg name");
            }
            state.args.emplace_back(StateArg::Type::sat_chr_var, lx.substring());
            lx.lex();
        } else {
            throw runtime_error("expected state arg or char arg");
        }

        tt = lx.toktype();
        if (tt == TokenType::rpar) {
            lx.lex();
            break;
        }
        lx.expect(TokenType::comma);
    }
}

void Parser::parseStatebody(State &s) {
    if (lx.toktype() == TokenType::rcurly)
        return;

    while (lx.toktype() == TokenType::lbracket) {
        lx.lex();
        s.branches.emplace_back();
        Branch &b = s.branches.back();
        parseCharargs(s, b);
        lx.expect(TokenType::rbracket);

        parsePrimitives(s, b.action);
        b.action.call = std::move(parseCall(s));
    }

    TokenType tt = lx.toktype();
    if (tt == TokenType::movel || tt == TokenType::mover
            || tt == TokenType::print || tt == TokenType::ident) {
        parsePrimitives(s, s.deflt);
        s.deflt.call = std::move(parseCall(s));
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

int find_arg(const State &s, const string &substring, StateArg::Type t) {
    int index = 0;
    for (const auto &arg : s.args) {
        if (arg.name == substring && arg.type == t)
            return index;
        ++index;
    }
    return -1;
}

CallArg Parser::parseChar(State &s) {
    TokenType tt = lx.toktype();
    if (tt == TokenType::chr_imm) {
        string repr = lx.substring();
        lx.lex();
        return CallArg(convert_immediate(repr));
    } else if (tt == TokenType::chr_var) {
        lx.lex();
        if (lx.toktype() != TokenType::ident)
            throw runtime_error("expected char argument name");
        string name = lx.substring();
        int index = find_arg(s, name, StateArg::Type::sat_chr_var);
        if (index == -1)
            throw runtime_error(name + "not recognized as char argument");
        lx.lex();
        return CallArg(index);
    }
    throw runtime_error("expected character");
}

void Parser::parsePrimitives(State &s, Action &a) {
    TokenType tt;

    while (lx.toktype() == TokenType::movel || lx.toktype() == TokenType::mover ||
            lx.toktype() == TokenType::print) {
        tt = lx.toktype();
        if (tt == TokenType::movel) {
            a.primitives.emplace_back(Primitive::Type::pt_movel);
            lx.lex();
        } else if (tt == TokenType::mover) {
            a.primitives.emplace_back(Primitive::Type::pt_mover);
            lx.lex();
        } else if (tt == TokenType::print) {
            lx.lex();
            tt = lx.toktype();
            if (tt == TokenType::chr_imm) {
                a.primitives.emplace_back(convert_immediate(lx.substring()));
                lx.lex();
            } else if (tt == TokenType::chr_var) {
                lx.lex();
                if (lx.toktype() != TokenType::ident)
                    throw runtime_error("expected char arg");
                string name = lx.substring();
                int index = find_arg(s, name, StateArg::Type::sat_chr_var);
                if (index == -1)
                    throw runtime_error(name + "not recognized as char argument");
                a.primitives.emplace_back(index);
                lx.lex();
            } else {
                throw runtime_error("expected char to print");
            }
        }
    }
}

void Parser::parseCharargs(State &s, Branch &b) {
    while (true) {
        b.chars.push_back(parseChar(s));
        if (lx.toktype() != TokenType::comma)
            break;
        lx.lex();
    }
}

unique_ptr<Call> Parser::parseCall(State &s) {
    if (lx.toktype() != TokenType::ident) {
        throw runtime_error("expected state name to parse call");
    }

    unique_ptr<Call> c = make_unique<Call>();

    string name = lx.substring();
    int index = find_arg(s, name, StateArg::Type::sat_state_var);
    if (index == -1) {
        c->type = Call::Type::ct_state_imm;
        c->name = name;
    } else {
        c->type = Call::Type::ct_state_var;
        c->index = index;
    }
    lx.lex();

    if (lx.toktype() == TokenType::lpar) {
        lx.lex();
        while (true) {
            TokenType tt = lx.toktype();
            if (tt == TokenType::chr_imm || tt == TokenType::chr_var) {
                c->args.push_back(parseChar(s));
            } else if (tt == TokenType::ident) {
                c->args.push_back(CallArg(parseCall(s)));
            } else {
                throw runtime_error("expected call arg");
            }
            tt = lx.toktype();
            if (tt == TokenType::comma) {
                lx.lex();
                continue;
            } else if (tt == TokenType::rpar) {
                lx.lex();
                break;
            } else {
                throw runtime_error("expected further argument or end of arg list");
            }
        }
    }

    return c;
}

} // namespace parse
