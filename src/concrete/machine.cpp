#include <iostream>

#include "concrete/machine.hpp"

using std::ostream;
using std::cout; using std::endl;
using std::string;
using std::set;

void Execution::step() {
    if (_term != TerminateType::cont)
        return;

    unsigned char sym = tape[_pos];
    const State &curr_state = m->get_state(state_idx);

    const Branch *curr_branch = nullptr;
    for (const auto &b : curr_state.branches) {
        if (b.def || b.syms.find(sym) != b.syms.end()) {
            curr_branch = &b;
            break;
        }
    }

    if (curr_branch == nullptr) { // no possible action
        _term = TerminateType::fail;
        return;
    }

    for (const auto &p : curr_branch->primitives) {
        switch (p.type) {
        case PrimitiveType::movel:
            --_pos;
            break;
        case PrimitiveType::mover:
            ++_pos;
            break;
        case PrimitiveType::print:
            tape[_pos] = p.sym;
            break;
        }
    }
    tape[_pos]; // update tape to fit
    
    _term = curr_branch->term;
    if (_term == TerminateType::cont) {
        state_idx = curr_branch->next;
    }
}

#define HEXDIGIT(n) (char)((n) < 10 ? (n) + '0' : (n) - 10 + 'a')
        
ostream &operator<<(ostream &os, const Primitive &p) {
    switch(p.type) {
    case PrimitiveType::movel:
        os << "<";
        break;
    case PrimitiveType::mover:
        os << ">";
        break;
    case PrimitiveType::print:
        os << "='x" << HEXDIGIT((p.sym >> 4) & 0x0f) << HEXDIGIT(p.sym & 0x0f) << "'";
        break;
    }
    return os;
}

ostream &Branch::out(ostream &os, const Machine &m) const {
    if (def) {
        os << "[def] ";
    } else {
        os << "[";
        set<unsigned char>::size_type i = 0;
        for (const auto &s: syms) {
            os << "'x" << HEXDIGIT((s >> 4) & 0x0f) << HEXDIGIT(s & 0x0f) << "'";
            if (++i != syms.size())
                os << ", ";
        }
        os << "] ";
    }
    for (const auto &p : primitives) {
        os << p << " ";
    }
    switch (term) {
    case TerminateType::cont:
        os << m.get_state(next).name;
        break;
    case TerminateType::accept:
        os << "accept";
        break;
    case TerminateType::reject:
        os << "reject";
        break;
    // fail should be impossible here
    case TerminateType::fail:
        os << "fail";
        break;
    }
    return os;
}

ostream &State::out(ostream &os, const Machine &m) const {
    os << name << " {" << endl;
    for (const auto &b : branches) {
        os << "    ";
        b.out(os, m) << endl;
    }
    os << "}";
    return os;
}

ostream &operator<<(ostream &os, const Machine &m) {
    for (const auto &s : m.states)
        s.out(os, m) << endl;
    return os;
}

ostream &operator<<(ostream &os, const Execution &ex) {
    Tape::size_type offset_zero = ex.tape.lsize();
    Tape::size_type offset = ex._pos + ex.tape.lsize();
    Tape::size_type i;
    for (i = 0; i != offset; ++i)
        os << " ";
    os << ex.m->get_state(ex.state_idx).name << endl;

    if (offset_zero == offset) {
        for (i = 0; i != offset; ++i)
            os << " ";
        os << "V" << endl;
    } else if (offset_zero < offset) {
        for (i = 0; i != offset_zero; ++i)
            os << " ";
        os << "|";
        for (i = offset_zero + 1; i != offset; ++i)
            os << " ";
        os << "V" << endl;
    } else {
        for (i = 0; i != offset; ++i)
            os << " ";
        os << "V";
        for (i = offset + 1; i != offset_zero; ++i)
            os << " ";
        os << "|" << endl;
    }
    os << ex.tape << endl;
    return os;
}

