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

void Sym::apply_sym(int arg_ind, unsigned char imm) {
    if (var && value == arg_ind) {
        var = false;
        value = imm;
    }
}

void CallArg::apply_sym(int arg_ind, unsigned char imm) {
    if (type == cat_sym) {
        this->sym.apply_sym(arg_ind, imm);
    } else if (call) {
        call->apply_sym(arg_ind, imm);
    }
}

void CallArg::apply_state(int arg_ind, const Call &newcall) {
    if (type == cat_call && call && call->type == Call::Type::ct_state_var &&
            call->index == arg_ind)
        call = make_unique<Call>(newcall);
    else if (type == cat_call && call && call->type == Call::Type::ct_state_imm)
        call->apply_state(arg_ind, newcall);
}

void Call::apply_sym(int arg_ind, unsigned char imm) {
    for (CallArg &ca : args)
        ca.apply_sym(arg_ind, imm);
}

void Call::apply_state(int arg_ind, const Call &newcall) {
    for (CallArg &ca : args) {
        ca.apply_state(arg_ind, newcall);
    }
}

void Primitive::apply_sym(int arg_ind, unsigned char imm) {
    if (type == pt_print) {
        sym.apply_sym(arg_ind, imm);
    }
}

void Action::apply_sym(int arg_ind, unsigned char imm) {
    for (Primitive &p : primitives)
        p.apply_sym(arg_ind, imm);
    if (call)
        call->apply_sym(arg_ind, imm);
}

void Action::apply_state(int arg_ind, const Call &newcall) {
    if (call && call->type == Call::Type::ct_state_var && call->index == arg_ind)
        call = make_unique<Call>(newcall);
    else if (call && call->type == Call::Type::ct_state_imm)
        call->apply_state(arg_ind, newcall);
}

void SymSpec::apply_sym(int arg_ind, unsigned char imm) {
    if (type == SymSpec::Type::ss_sym) {
        left.apply_sym(arg_ind, imm);
    } else if (type == SymSpec::Type::ss_symrange) {
        left.apply_sym(arg_ind, imm);
        right.apply_sym(arg_ind, imm);
    }
}

void Branch::apply_sym(int arg_ind, unsigned char imm) {
    for (SymSpec &ss : syms)
        ss.apply_sym(arg_ind, imm);
    action.apply_sym(arg_ind, imm);
}

void Branch::apply_state(int arg_ind, const Call &newcall) {
    action.apply_state(arg_ind, newcall);
}

void State::apply_sym(unsigned char imm) {
    if (subs.size() == args.size())
        throw runtime_error("all arguments already applied");
    if (args[subs.size()].type != StateArg::Type::sat_sym_var)
        throw runtime_error("try to replace sym arg with state");
    for (Branch &b : branches)
        b.apply_sym(subs.size(), imm);
    subs.emplace_back(imm);
}

void State::apply_state(const Call &newcall) {
    if (subs.size() == args.size())
        throw runtime_error("all arguments already applied");
    if (args[subs.size()].type != StateArg::Type::sat_state_var)
        throw runtime_error("try to replace state arg with sym");
    for (Branch &b : branches)
        b.apply_state(subs.size(), newcall);
    subs.emplace_back(make_unique<Call>(newcall));
}

CallArg::CallArg(const CallArg &that): type(that.type), sym(that.sym), call(nullptr) {
    if (that.call)
        call = make_unique<Call>(*(that.call));
}

CallArg &CallArg::operator=(const CallArg &that) {
    type = that.type;
    sym = that.sym;
    call = (that.call) ? make_unique<Call>(*(that.call)) : nullptr;
    return *this;
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

Substitute::Substitute(const Substitute &that): imm(that.imm), call(nullptr) { 
    if (that.call)
        call = make_unique<Call>(*(that.call));
}

Substitute &Substitute::operator=(const Substitute &that) {
    imm = that.imm;
    call = (that.call) ? make_unique<Call>(*(that.call)) : nullptr;
    return *this;
}

void resolve_call(const Call &call, ostream &os, const vector<State> &states, set<string> &expandedstates) {
    const State *sp = nullptr;
    for (auto &s : states)
        if (s.name == call.name)
            sp = &s;
    if (!sp)
        throw runtime_error("state name to expand not found: " + call.name);
    State statecopy = *sp;
    for (const auto &ca : call.args) {
        if (ca.type == CallArg::Type::cat_sym) {
            if (ca.sym.var) {
                throw runtime_error("unexpected unresolved char");
            }
            statecopy.apply_sym(ca.sym.value);
        } else {
            statecopy.apply_state(*(ca.call));
        }
    }
    statecopy.expand(os, states, expandedstates);
}

string hexstring(int value) {
    string s = "'x";
    char high = (value >> 4) & 0x0f;
    char low = value & 0x0f;
    s.push_back((high >= 10) ? 'a' + (high - 10) : '0' + high);
    s.push_back((low >= 10) ? 'a' + (low - 10) : '0' + low);
    s.push_back('\'');
    return s;
}

string State::expandedname() const {
    if (args.size() == 0)
        return name;
    
    ostringstream os;
    os << name << "(";
    for (int i = 0; i != subs.size(); ++i) {
        if (args[i].type == StateArg::Type::sat_sym_var)
            os << hexstring(subs[i].imm);
        else
            os << *(subs[i].call);
        if (i != args.size() - 1)
            os << ", ";
    }
    for (int i = subs.size(); i != args.size(); ++i) {
        os << args[i];
        if (i != args.size() - 1)
            os << ", ";
    }
    os << ")";
    return os.str();
}

// TODO might not need substitute vector because we substitute directly
// TODO or: more efficient solution possible without excessive copying?

ostream &state_out_custom(ostream &os, const State &s);

void State::expand(ostream &os, const vector<State> &states, set<string> &expandedstates) {
    if (args.size() != subs.size())
        throw runtime_error("state is not fully substituted");
    string en = expandedname();
    if (expandedstates.find(en) != expandedstates.end())
        return;
    expandedstates.insert(en);
    state_out_custom(os, *this) << "\n\n";

    for (Branch &b : branches) {
        if (b.action.call && b.action.call->type != Call::Type::ct_state_accept && b.action.call->type != Call::Type::ct_state_reject)
            resolve_call(*(b.action.call), os, states, expandedstates);
    }
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

ostream &operator<<(ostream &os, const Sym &s) {
    if (s.var) {
        os << "$[" << s.value << "]";
    } else {
        os << hexstring(s.value);
    }
    return os;
}

ostream &operator<<(ostream &os, const StateArg &sa) {
    if (sa.type == StateArg::Type::sat_sym_var)
        os << "$";
    os << sa.name;
    return os;
}

ostream &operator<<(ostream &os, const CallArg &ca) {
    if (ca.type == CallArg::Type::cat_sym) {
        os << ca.sym;
    } else {
        os << *(ca.call);
    }
    return os;
}

ostream &operator<<(ostream &os, const Call &c) {
    if (c.type == Call::Type::ct_state_var) {
        os << "$[" << c.index << "]";
    } else if (c.type == Call::Type::ct_state_imm) {
    	os << c.name;
    	if (c.args.size() > 0) {
    	    output_csl(os, c.args, "(", ", ", ")");
    	}
    } else if (c.type == Call::Type::ct_state_accept) {
        os << "accept";
    } else if (c.type == Call::Type::ct_state_reject) {
        os << "reject";
    }
    return os;
}

ostream &operator<<(ostream &os, const Primitive &p) {
    if (p.type == Primitive::Type::pt_movel)
        os << "<";
    else if (p.type == Primitive::Type::pt_mover)
        os << ">";
    else
        os << "=" << p.sym;
    return os;
}

ostream &operator<<(ostream &os, const Action &a) {
    for (const Primitive &p : a.primitives)
        os << p;
    if (a.primitives.size() > 0)
    	os << " ";
    if (a.call) {
        os << *(a.call);
        
    }
    return os;
}

ostream &operator<<(ostream &os, const SymSpec &ss) {
    if (ss.type == SymSpec::Type::ss_sym) {
        os << ss.left;
    } else if (ss.type == SymSpec::Type::ss_symrange) {
        os << ss.left << "-" << ss.right;
    } else {
        os << "def";
    }
    return os;
}

ostream &operator<<(ostream &os, const Branch &b) {
    output_csl(os, b.syms, "[", ", ", "]");
    os << " " << b.action;
    return os;
}

ostream &operator<<(ostream &os, const State &s) {
    os << s.expandedname() << " {\n";
    if (s.branches.size() > 0)
        output_csl(os, s.branches, "  ", "\n  ", "\n");
    os << "}";
    return os;
}

ostream &state_out_custom(ostream &os, const State &s) {
    os << "\"" << s.expandedname() << "\" {\n";
    for (const Branch &b : s.branches) {
        output_csl(os, b.syms, "  [", ", ", "]");
    	os << " ";
    	for (const Primitive &p : b.action.primitives)
            os << p;
        if (b.action.primitives.size() > 0)
    	    os << " ";
    	// custom usage of quotes
    	if (b.action.call) {
    	    if (b.action.call->type == Call::Type::ct_state_imm) {
    	        os << "\"" << *(b.action.call) << "\"";
    	    } else {
    	        os << *(b.action.call);
    	    }
    	}
    	os << "\n";
    }
    os << "}";
    return os;
}
