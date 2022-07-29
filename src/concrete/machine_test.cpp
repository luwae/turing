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
    State add;
    Branch b1, b2, b3, b4;

    main.name = "main";
    b1.syms.insert('_');
    b1.action.primitives.emplace_back(PrimitiveType::pt_movel);
    b1.action.term = TerminateType::term_cont;
    b1.action.next = 1;
    main.branches.push_back(b1);
    b2.symset_invert = true;
    b2.action.primitives.emplace_back(PrimitiveType::pt_mover);
    b2.action.term = TerminateType::term_cont;
    b2.action.next = 0;
    main.branches.push_back(b2);
    tm.add_state(main);

    add.name = "add";
    b3.syms.insert('1');
    b3.action.primitives.emplace_back(PrimitiveType::pt_print, '0');
    b3.action.primitives.emplace_back(PrimitiveType::pt_movel);
    b3.action.term = TerminateType::term_cont;
    b3.action.next = 1;
    add.branches.push_back(b3);
    b4.symset_invert = true;
    b4.action.primitives.emplace_back(PrimitiveType::pt_print, '1');
    b4.action.term = TerminateType::term_acc;
    add.branches.push_back(b4);
    tm.add_state(add);

    cout << tm << endl;
    
    MachineExecution me(&tm, "1011");
    while (me.get_term() == TerminateType::term_cont) {
        me.step();
        cout << me << endl;
    }
}
