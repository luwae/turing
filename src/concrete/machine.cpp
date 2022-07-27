#include <iostream>

#include "machine.hpp"

using std::ostream;
using std::cout; using std::endl;
using std::string;

void MachineExecution::step() {
    if (term != TerminateType::term_cont)
        return;

    unsigned char chr = tape[pos];
    const State &curr = tm->get_state(state);

    const Action *a = &(curr.deflt);
    for (const auto &b : curr.branches) {
        if (b.chars.find(chr) != b.chars.end()) {
            a = &(b.action);
            break;
        }
    }

    for (const auto &p : a->primitives) {
        switch (p.type) {
        case PrimitiveType::pt_movel:
            --pos;
            break;
        case PrimitiveType::pt_mover:
            ++pos;
            break;
        case PrimitiveType::pt_print:
            tape[pos] = p.chr;
            break;
        }
    }
    tape[pos]; // update tape to fit
    
    term = a->term;
    if (term == TerminateType::term_cont) {
        state = a->next;
    }
}

std::ostream &operator<<(std::ostream &os, const TuringMachine &tm) {
    for (const auto &s : tm.states)
        os << s << "\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const MachineExecution &me) {
    int offset_zero = me.tape.lsize();
    int offset = me.pos + me.tape.lsize();
    for (int i = 0; i != offset; ++i)
        os << " ";
    os << me.tm->get_state(me.state).name << "\n";

    if (offset_zero == offset) {
        for (int i = 0; i != offset; ++i)
            os << " ";
        os << "V\n";
    } else if (offset_zero < offset) {
        for (int i = 0; i != offset_zero; ++i)
            os << " ";
        os << "|";
        for (int i = offset_zero + 1; i != offset; ++i)
            os << " ";
        os << "V\n";
    } else {
        for (int i = 0; i != offset; ++i)
            os << " ";
        os << "V";
        for (int i = offset + 1; i != offset_zero; ++i)
            os << " ";
        os << "|\n";
    }
    os << me.tape << "\n";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Primitive &p) {
    switch(p.type) {
    case PrimitiveType::pt_movel:
        os << "<";
        break;
    case PrimitiveType::pt_mover:
        os << ">";
        break;
    case PrimitiveType::pt_print:
        os << "='" << p.chr << "'";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Action &a) {
    os << "Action(";
    for (const auto &p : a.primitives)
        os << p;
    os << ", ";
    switch (a.term) {
    case term_cont:
        os << a.next;
        break;
    case term_acc:
        os << "<accept>";
        break;
    case term_rej:
        os << "<reject>";
        break;
    }
    os << ")";
    return os;
}

template <typename T>
void output_csl(std::ostream &os, const T &container) {
    typename T::size_type i = 0;
    for (const auto &elem : container) {
        os << elem;
        if (++i != container.size())
            os << ", ";
    }
}

std::ostream &operator<<(std::ostream &os, const Branch &b) {
    os << "[";
    output_csl(os, b.chars);
    os << "]: " << b.action;
    return os;
}

std::ostream &operator<<(std::ostream &os, const State &s) {
    os << "State::" << s.name << "(";
    output_csl(os, s.branches);
    os << ", default=" << s.deflt << ")";
    return os;
}
