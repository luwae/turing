#include <iostream>
#include <fstream>
#include <string>

#include "ctm.hpp"

using std::cout; using std::endl;
using std::ifstream;
using std::string;

string hex(char sc) {
    unsigned char c = (unsigned char) sc;
    unsigned char upper = (c >> 4) & 0x0f, lower = c & 0x0f;
    string s;
    s.push_back((upper < 10) ? upper + '0' : upper - 10 + 'a');
    s.push_back((upper < 10) ? lower + '0' : lower - 10 + 'a');
    return s;
}

int main() {
    ifstream in("add1.ctm");
    TuringMachine tm;

    char c;
    int status = ctm_decode(tm, in);
    cout << status << endl;
    if (status) {
        cout << "remaining: " << endl;
        while (in.get(c))
            cout << hex(c) << " ";
        cout << endl;
    } else {
        cout << tm << endl;
    }
}
