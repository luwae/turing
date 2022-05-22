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
        for (const auto &s : states)
            if (s.name == name)
                throw runtime_error("duplicate state name");
        State s;
        s.name = name;
        parseStateargs(s);
        lx.expect(TokenType::lcurly);
        parseStatebody(s);
        lx.expect(TokenType::rcurly);
        states.push_back(s);
    }
    
}

void Parser::parseStateargs(State &state) {
    if (lx.toktype() == TokenType::lcurly)
        return;
    lx.expect(TokenType::lpar);
    while (true) {
        TokenType tt = lx.toktype();
        if (tt == TokenType::ident) {
           state.args.emplace_back(StateArgType::sat_state_var, lx.substring());
            lx.lex();
        } else if (tt == TokenType::chr_var) {
            lx.lex();
            if (lx.toktype() != TokenType::ident) {
               throw runtime_error("expected char arg name");
            }
            state.args.emplace_back(StateArgType::sat_chr_var, lx.substring());
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
        Branch b;
        parseCharargs(s, b);
        lx.expect(TokenType::rbracket);

        parsePrimitives(s, b.action);
        b.action.call = std::unique_ptr<Call>(parseCall(s));
        s.branches.push_back(b);
    }

    TokenType tt = lx.toktype();
    if (tt == TokenType::movel || tt == TokenType::mover
            || tt == TokenType::print || tt == TokenType::ident) {
        parsePrimitives(s, s.deflt);
        s.deflt.call = std::unique_ptr<Call>(parseCall(s));
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

int find_arg(const State &s, const string &substring, StateArgType t) {
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
        int index = find_arg(s, name, StateArgType::sat_chr_var);
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
            a.primitives.emplace_back(PrimitiveType::pt_movel);
            lx.lex();
        } else if (tt == TokenType::mover) {
            a.primitives.emplace_back(PrimitiveType::pt_mover);
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
                int index = find_arg(s, name, StateArgType::sat_chr_var);
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

Call *Parser::parseCall(State &s) {
    if (lx.toktype() != TokenType::ident) {
        throw runtime_error("expected state name to parse call");
    }

    Call *c = new Call;

    string name = lx.substring();
    int index = find_arg(s, name, StateArgType::sat_state_var);
    if (index == -1) {
        c->type = CallType::ct_state_imm;
        c->name = name;
        lx.lex();
    } else {
        c->type = CallType::ct_state_var;
        c->index = index;
        lx.lex();
        // can't pass args to variable state
        // maybe we want this??? don't see what could go wrong
        return c;
    }

    if (lx.toktype() == TokenType::lpar) {
        lx.lex();
        while (true) {
            TokenType tt = lx.toktype();
            if (tt == TokenType::chr_imm || tt == TokenType::chr_var) {
                c->args.push_back(parseChar(s));
            } else if (tt == TokenType::ident) {
                c->args.push_back(CallArg(std::unique_ptr<Call>(parseCall(s))));
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

const char *stateArgNames[] = {"chr_var", "state_var"};

ostream &operator<<(ostream &os, const StateArg &sa) {
    os << "StateArg(" << stateArgNames[sa.type] << ", " << sa.name << ")";
    return os;
}

const char *callArgNames[] = {"chr_var", "chr_imm", "call"};

ostream &operator<<(ostream &os, const CallArg &ca) {
    os << "CallArg(" << callArgNames[ca.type] << ", ";
    if (ca.type == CallArgType::cat_chr_var)
        os << "index=" << ca.index << ")";
    else if (ca.type == CallArgType::cat_chr_imm)
        os << "char=" << ca.imm << ")";
    else
        os << "call=" << *(ca.call) << ")";
    return os;
}

const char *callNames[] = {"state_var", "state_imm"};

ostream &operator<<(ostream &os, const Call &c) {
    os << "Call(" << callNames[c.type] << ", ";
    if (c.type == CallType::ct_state_var)
        os << "index=" << c.index;
    else
        os << "name=" << c.name;
    os << ", {";
    for (auto &arg : c.args)
        os << arg << ", ";
    os << "})";
    return os;
}

const char *primitiveNames[] = {"<", ">", "="};

ostream &operator<<(ostream &os, const Primitive &p) {
    if (p.type == PrimitiveType::pt_movel || p.type == PrimitiveType::pt_mover)
        os << primitiveNames[p.type];
    else
        os << "=" << p.arg;
    return os;
}

ostream &operator<<(ostream &os, const Action &a) {
    os << "Action({";
    for (auto &p : a.primitives)
        os << p << ", ";
    if (a.call)
        os << "}, " << *(a.call) << ")";
    else
        os << "}, <null>)";
    return os;
}

ostream &operator<<(ostream &os, const Branch &b) {
    os << "Branch([";
    for (auto &chr : b.chars)
        os << chr << ", ";
    os << "], " << b.action << ")";
    return os;
}

ostream &operator<<(ostream &os, const State &s) {
    os << "State::" << s.name << "({";
    for (auto &arg : s.args)
        os << arg << ", ";
    os << "}, {";
    for (auto &b : s.branches)
        os << b << ", ";
    os << "}, default=" << s.deflt << ")";
    return os;
}

} // namespace parse
