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

int main() {
    State mvr;
    mvr.name = "mvr";
    Branch mvr_01;
    mvr_01.chars.insert('0');
    mvr_01.chars.insert('1');
    mvr_01.action.primitives.emplace_back(PrimitiveType::pt_mover, 0);
    mvr_01.action.next = "mvr";
    mvr.branches.push_back(mvr_01);
    mvr.deflt.primitives.emplace_back(PrimitiveType::pt_movel, 0);
    mvr.deflt.next = "mvl";

    State mvl;
    mvl.name = "mvl";
    Branch mvl_1;
    mvl_1.chars.insert('1');
    mvl_1.action.primitives.emplace_back(PrimitiveType::pt_print, '0');
    mvl_1.action.primitives.emplace_back(PrimitiveType::pt_movel, 0);
    mvl_1.action.next = "mvl";
    mvl.branches.push_back(mvl_1);
    mvl.deflt.primitives.emplace_back(PrimitiveType::pt_print, '1');
    mvl.deflt.next = "end";

    State end;
    end.name = "end";

    TuringMachine tm;
    tm.states.push_back(mvr);
    tm.states.push_back(mvl);
    tm.states.push_back(end);
    tm.statemap.insert({"mvr", 0});
    tm.statemap.insert({"mvl", 1});
    tm.statemap.insert({"end", 2});

    Tape t("1111");

    tm.print(t);
    cout << endl;
    while (!tm.done) {
        tm.step(t);
        tm.print(t);
        cout << endl;
    }
}
