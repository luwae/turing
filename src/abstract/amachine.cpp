#include <stdexcept>
#include <sstream>

#include "amachine.hpp"

using std::string;
using std::ostream;
using std::unique_ptr; using std::make_unique;
using std::set;
using std::runtime_error;
using std::ostringstream;
using std::vector;

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

void CallArg::apply_state(int arg_ind, const Call &newcall) {
    if (type == cat_call && call && call->type == Call::Type::ct_state_var &&
            call->index == arg_ind)
        call = make_unique<Call>(newcall);
    else if (type == cat_call && call && call->type == Call::Type::ct_state_imm)
        call->apply_state(arg_ind, newcall);
}

void Call::apply_chr(int arg_ind, unsigned char imm) {
    for (auto &ca : args)
        ca.apply_chr(arg_ind, imm);
}

void Call::apply_state(int arg_ind, const Call &newcall) {
    for (auto &ca : args) {
        ca.apply_state(arg_ind, newcall);
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

void Action::apply_state(int arg_ind, const Call &newcall) {
    if (call && call->type == Call::Type::ct_state_var && call->index == arg_ind)
        call = make_unique<Call>(newcall);
    else if (call && call->type == Call::Type::ct_state_imm)
        call->apply_state(arg_ind, newcall);
}

void Branch::apply_chr(int arg_ind, unsigned char imm) {
    for (auto &ca : chars)
        ca.apply_chr(arg_ind, imm);
    action.apply_chr(arg_ind, imm);
}

void Branch::apply_state(int arg_ind, const Call &newcall) {
    action.apply_state(arg_ind, newcall);
}

Substitute::Substitute(const Substitute &that): imm(that.imm), call(nullptr) { 
    if (that.call)
        call = make_unique<Call>(*(that.call));
}

Substitute &Substitute::operator=(const Substitute &that) {
    imm = that.imm;
    call = (that.call) ? make_unique<Call>(*(that.call)) : nullptr;
    return *this;
}

void State::apply_chr(unsigned char imm) {
    if (subs.size() == args.size())
        throw runtime_error("all arguments already applied");
    if (args[subs.size()].type != StateArg::Type::sat_chr_var)
        throw runtime_error("try to replace char arg with state");
    for (auto &b : branches)
        b.apply_chr(subs.size(), imm);
    deflt.apply_chr(subs.size(), imm);
    subs.emplace_back(imm, nullptr);
}

void State::apply_state(const Call &newcall) {
    if (subs.size() == args.size())
        throw runtime_error("all arguments already applied");
    if (args[subs.size()].type != StateArg::Type::sat_state_var)
        throw runtime_error("try to replace state arg with char");
    for (auto &b : branches)
        b.apply_state(subs.size(), newcall);
    deflt.apply_state(subs.size(), newcall);
    subs.emplace_back(0, make_unique<Call>(newcall));
}

ostream &xchr(ostream &os, unsigned char imm) {
    os << "'x";
    static const string hexchars = "0123456789abcdef";
    os << hexchars[imm >> 4];
    os << hexchars[imm & 0x0f];
    os << "'";
    return os;
}

string State::rname() const {
    if (args.size() == 0)
        return name;
    
    ostringstream os;
    os << name << "(";
    for (int i = 0; i != subs.size(); ++i) {
        if (args[i].type == StateArg::Type::sat_chr_var)
            xchr(os, subs[i].imm);
        else
            os << *(subs[i].call);
        if (i != args.size() - 1)
            os << ",";
    }
    for (int i = subs.size(); i != args.size(); ++i) {
        os << args[i];
        if (i != args.size() - 1)
            os << ",";
    }
    os << ")";
    return os.str();
}

void resolve_call(const Call &call, ostream &os, const vector<State> &states, set<string> &rstates) {
    const State *sp = nullptr;
    for (auto &s : states)
        if (s.name == call.name)
            sp = &s;
    if (!sp)
        throw runtime_error("state name to expand not found");
    State statecopy = *sp;
    for (const auto &ca : call.args) {
        if (ca.type == CallArg::Type::cat_chr_imm)
            statecopy.apply_chr(ca.imm);
        else if (ca.type == CallArg::Type::cat_call)
            statecopy.apply_state(*(ca.call));
        else
            throw runtime_error("unexpected unresolved char");
    }
    statecopy.expand(os, states, rstates);
}

void State::expand(ostream &os, const vector<State> &states, set<string> &rstates) {
    if (args.size() != subs.size())
        throw runtime_error("state is not fully substituted");
    string rn = rname();
    if (rstates.find(rn) != rstates.end())
        return;
    rstates.insert(rn);
    os << *this << "\n";

    for (auto &b : branches)
        if (b.action.call)
            resolve_call(*(b.action.call), os, states, rstates);
    if (deflt.call)
        resolve_call(*(deflt.call), os, states, rstates);
}

template <typename T>
ostream &output_csl(std::ostream &os, const T &container, const string &before, const string &mid, const string &after) {
    typename T::size_type i = 0;

    os << before;
    for (const auto &elem : container) {
        os << elem;
        if (++i != container.size())
            os << mid;
    }
    os << after;
    return os;
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
    else if (ca.type == CallArg::Type::cat_chr_imm) {
        xchr(os, ca.imm);
    } else
        os << *(ca.call);
    return os;
}

ostream &operator<<(ostream &os, const Call &c) {
    if (c.type == Call::Type::ct_state_var)
        os << "<" << c.index << ">";
    else
        os << c.name;
    if (c.args.size() > 0)
        output_csl(os, c.args, "(", ",", ")");
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
    output_csl(os, b.chars, "[", ",", "]");
    os << " " << b.action;
    return os;
}

ostream &operator<<(ostream &os, const State &s) {
    os << s.rname() << " {\n";
    if (s.branches.size() > 0)
        output_csl(os, s.branches, "  ", "\n  ", "\n");
    if (s.deflt.call)
        os << "  " << s.deflt << "\n";
    os << "}";
    return os;
}
