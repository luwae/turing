#include <iostream>

#include "machine.hpp"

using std::cout; using std::endl;

/*
main {
  ['_'] < add
  > main
}

add {
  ['0', '_'] ='1' ACCEPT
  ='0' < add
}
 */

int main() {
    TuringMachine tm;
    State main;
    Branch b;

    main.name = "main";
    b.chars.insert('_');
    b.action.primitives.emplace_back(PrimitiveType::pt_movel);
    b.action.term = TerminateType::term_cont;
    b.action.next = 1;
    main.branches.push_back(b);
    main.deflt.primitives.emplace_back(PrimitiveType::pt_mover);
    main.deflt.term = TerminateType::term_cont;
    main.deflt.next = 0;
    tm.add_state(main);

    State add;
    Branch b2;

    add.name = "add";
    b2.chars.insert('_');
    b2.chars.insert('0');
    b2.action.primitives.emplace_back(PrimitiveType::pt_print, '1');
    b2.action.term = TerminateType::term_acc;
    add.branches.push_back(b2);
    add.deflt.primitives.emplace_back(PrimitiveType::pt_print, '0');
    add.deflt.primitives.emplace_back(PrimitiveType::pt_movel);
    add.deflt.term = TerminateType::term_cont;
    add.deflt.next = 1;
    tm.add_state(add);

    cout << tm << endl;
    
    MachineExecution me(&tm, "1011");
    while (me.get_term() == TerminateType::term_cont) {
        me.step();
        cout << me << endl;
    }
    cout << ((me.get_term() == TerminateType::term_acc) ? "accept" : "reject") << endl;
}
