#include <iostream>

#include "machine.hpp"

using std::ostream;
using std::cout; using std::endl;

void TuringMachine::step(Tape &t) {
    if (done)
        return;

    unsigned char chr = t[pos];
    State &curr = states[state];

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
            t[pos] = p.chr;
            break;
        }
    }
    t[pos]; // update tape to fit
    
    auto f = statemap.find(a->next);
    if (f == statemap.end())
        done = true;
    else
        state = f->second;
}

void TuringMachine::print(const Tape &t) const {
    int offset_zero = t.lsize();
    int offset = pos + t.lsize();
    for (int i = 0; i != offset; ++i)
        cout << " ";
    cout << states[state].name << "\n";

    if (offset_zero == offset) {
        for (int i = 0; i != offset; ++i)
            cout << " ";
        cout << "V\n";
    } else if (offset_zero < offset) {
        for (int i = 0; i != offset_zero; ++i)
            cout << " ";
        cout << "|";
        for (int i = offset_zero + 1; i != offset; ++i)
            cout << " ";
        cout << "V\n";
    } else {
        for (int i = 0; i != offset; ++i)
            cout << " ";
        cout << "V";
        for (int i = offset + 1; i != offset_zero; ++i)
            cout << " ";
        cout << "|\n";
    }
    cout << t << "\n";
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
    os << ", " << a.next << ")";
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

std::ostream &operator<<(std::ostream &os, const TuringMachine &tm) {
    for (const auto &s : tm.states)
        os << s << "\n";
    return os;
}
