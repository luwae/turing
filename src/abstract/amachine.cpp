#include <stdexcept>

#include "amachine.hpp"

using std::string;
using std::ostream;
using std::unique_ptr; using std::make_unique;
using std::set;
using std::runtime_error;

CallArg::CallArg(const CallArg &that): type(that.type), index(that.index), imm(that.imm), call(nullptr) {
    if (that.call)
        call = make_unique<Call>(*(that.call));
}

CallArg &CallArg::operator=(const CallArg &that) {
    type = that.type;
    index = that.index;
    imm = that.imm;
    call = (that.call) ? make_unique<Call>(*(that.call)) : nullptr;
    return *this;
}

void CallArg::apply_chr(int arg_ind, unsigned char imm) {
    if (type == cat_chr_var) {
        if (arg_ind == index) {
            type = cat_chr_imm;
            this->imm = imm;
        }
    } else if (type == cat_call && call) {
        call->apply_chr(arg_ind, imm);
    }
}

void CallArg::apply_state(int arg_ind, const string &name) {
    if (type == cat_call && call)
        call->apply_state(arg_ind, name);
}

void Call::apply_chr(int arg_ind, unsigned char imm) {
    for (auto &ca : args)
        ca.apply_chr(arg_ind, imm);
}

void Call::apply_state(int arg_ind, const string &name) {
    if (type == ct_state_var) {
        if (arg_ind == index) {
            type = ct_state_imm;
            this->name = name;
        }
    }
    for (auto &ca : args) {
        ca.apply_state(arg_ind, name);
    }
}

void Primitive::apply_chr(int arg_ind, unsigned char imm) {
    if (type == pt_print) {
        arg.apply_chr(arg_ind, imm);
    }
}

Action::Action(const Action &that): primitives(that.primitives), call(nullptr) { 
    if (that.call)
        call = make_unique<Call>(*(that.call));
}

Action &Action::operator=(const Action &that) {
    primitives = that.primitives;
    call = (that.call) ? make_unique<Call>(*(that.call)) : nullptr;
    return *this;
}

void Action::apply_chr(int arg_ind, unsigned char imm) {
    for (auto &p : primitives)
        p.apply_chr(arg_ind, imm);
    if (call)
        call->apply_chr(arg_ind, imm);
}

void Action::apply_state(int arg_ind, const string &name) {
    if (call)
        call->apply_state(arg_ind, name);
}

void Branch::apply_chr(int arg_ind, unsigned char imm) {
    for (auto &ca : chars)
        ca.apply_chr(arg_ind, imm);
    action.apply_chr(arg_ind, imm);
}

void Branch::apply_state(int arg_ind, const string &name) {
    for (auto &ca : chars)
        ca.apply_state(arg_ind, name);
    action.apply_state(arg_ind, name);
}

void State::apply_chr(unsigned char imm) {
    if (applied == args.size())
        throw runtime_error("all arguments already applied");
    if (args[applied].type != StateArg::Type::sat_chr_var)
        throw runtime_error("try to replace char arg with state");
    for (auto &b : branches)
        b.apply_chr(applied, imm);
    deflt.apply_chr(applied, imm);
    ++applied;
}

void State::apply_state(const std::string &name) {
    if (applied == args.size())
        throw runtime_error("all arguments already applied");
    if (args[applied].type != StateArg::Type::sat_state_var)
        throw runtime_error("try to replace state arg with char");
    for (auto &b : branches)
        b.apply_state(applied, name);
    deflt.apply_state(applied, name);
    ++applied;
}

string State::rname() {
    string s = name;
    // TODO
    return s;
}

void State::expand(ostream &os, set<string> exp) {
    if (applied != args.size())
        throw runtime_error("not fully substituted");
    // TODO
}



template <typename T>
void output_csl(std::ostream &os, const T &container, const string &before, const string &after) {
    typename T::size_type i = 0;

    os << before;
    for (const auto &elem : container) {
        os << elem;
        if (++i != container.size())
            os << ", ";
    }
    os << after;
}

ostream &operator<<(ostream &os, const StateArg &sa) {
    if (sa.type == StateArg::Type::sat_chr_var)
        os << "$";
    os << sa.name;
    return os;
}

ostream &operator<<(ostream &os, const CallArg &ca) {
    if (ca.type == CallArg::Type::cat_chr_var)
        os << "$<" << ca.index << ">";
    else if (ca.type == CallArg::Type::cat_chr_imm)
        os << "'" << ca.imm << "'";
    else
        os << *(ca.call);
    return os;
}

ostream &operator<<(ostream &os, const Call &c) {
    if (c.type == Call::Type::ct_state_var)
        os << "<" << c.index << ">";
    else
        os << c.name;
    output_csl(os, c.args, "(", ")");
    return os;
}

ostream &operator<<(ostream &os, const Primitive &p) {
    if (p.type == Primitive::Type::pt_movel)
        os << "<";
    else if (p.type == Primitive::Type::pt_mover)
        os << ">";
    else
        os << "=" << p.arg;
    return os;
}

ostream &operator<<(ostream &os, const Action &a) {
    for (const auto &p : a.primitives)
        os << p;
    if (a.call)
        os  << *(a.call); 
    return os;
}

ostream &operator<<(ostream &os, const Branch &b) {
    output_csl(os, b.chars, "[", "]");
    os << " " << b.action;
    return os;
}

ostream &operator<<(ostream &os, const State &s) {
    os << s.name;
    if (s.applied != s.args.size()) {
        os << "(";
        int i = 0;
        for (auto it = s.args.begin() + s.applied; it != s.args.end(); ++it) {
            os << *it;
            if (++i != s.args.size() - s.applied)
                os << ", ";
        }
        os << ")";
    }
    os << "(";
    output_csl(os, s.branches, "{", "}");
    os << ", ";
    os << s.deflt << ")";
    return os;
}
