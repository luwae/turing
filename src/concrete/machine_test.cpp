#include <iostream>

#include "concrete/machine.hpp"

using std::cout; using std::endl;

/*
main {
  ['_'] < add
  [def] >
}

add {
  ['1'] ='0' <
  [def] ='1' accept
}
 */

int main(int argc, char *argv[]) {
    Machine m;
    State main;
    State add;
    Branch b1, b2, b3, b4;

    main.name = "main";
    b1.syms.insert('_');
    b1.primitives.emplace_back(PrimitiveType::movel);
    b1.term = TerminateType::cont;
    b1.next = 1;
    main.branches.push_back(b1);
    b2.def = true;
    b2.primitives.emplace_back(PrimitiveType::mover);
    b2.term = TerminateType::cont;
    b2.next = 0;
    main.branches.push_back(b2);
    m.add_state(main);

    add.name = "add";
    b3.syms.insert('1');
    b3.primitives.emplace_back(PrimitiveType::print, '0');
    b3.primitives.emplace_back(PrimitiveType::movel);
    b3.term = TerminateType::cont;
    b3.next = 1;
    add.branches.push_back(b3);
    b4.def = true;
    b4.primitives.emplace_back(PrimitiveType::print, '1');
    b4.term = TerminateType::accept;
    add.branches.push_back(b4);
    m.add_state(add);

    cout << m << endl;
    
    if (argc < 2) {
        cout << "enter initial tape state to see machine execution" << endl;
        return 0;
    }
    Execution ex(&m, argv[1]);
    while (ex.term() == TerminateType::cont) {
        ex.step();
        cout << ex << endl;
    }
}
