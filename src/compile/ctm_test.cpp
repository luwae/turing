#include <iostream>
#include <fstream>

#include "ctm.hpp"

using std::cout; using std::endl;
using std::ifstream;

int main() {
    ifstream in("add1.ctm");
    TuringMachine tm;

    int status = ctm_decode(tm, in);
    cout << status << endl;
    if (!status)
        cout << tm << endl;
}
