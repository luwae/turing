#include <iostream>

#include "machine.hpp"

using std::cout; using std::endl;

/*
main {
  ['_'] < add
  > main
}

end { }

add {
  ['0', '_'] ='1' end
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
    b.action.next = "add";
    main.branches.push_back(b);
    main.deflt.primitives.emplace_back(PrimitiveType::pt_mover);
    main.deflt.next = "main";
    tm.add_state(main);

    State end;
    end.name = "end";
    tm.add_state(end);

    State add;
    Branch b2;

    add.name = "add";
    b2.chars.insert('_');
    b2.chars.insert('0');
    b2.action.primitives.emplace_back(PrimitiveType::pt_print, '1');
    b2.action.next = "end";
    add.branches.push_back(b2);
    add.deflt.primitives.emplace_back(PrimitiveType::pt_print, '0');
    add.deflt.primitives.emplace_back(PrimitiveType::pt_movel);
    add.deflt.next = "add";
    tm.add_state(add);

    cout << tm << endl;
    
    MachineExecution me(&tm, "1011");
    while (!me.isDone()) {
        me.step();
        cout << me << endl;
    }
}
