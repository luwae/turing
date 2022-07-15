#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "parse.hpp"

using std::cin; using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using parse::Parser;
using std::string;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "usage: turing <machine file> <tape input>" << endl;
        return -1;
    }

    ifstream file(argv[1]);
    stringstream ss;
    ss << file.rdbuf();
    string input = ss.str();

    cout << "raw machine:\n";
    cout << input << endl;
   
    TuringMachine tm;
    Parser ps(input, tm);
    cout << "finished parsing" << endl;

    MachineExecution me(&tm, argv[2]);
    while (!me.isDone()) {
        me.step();
        cout << me << endl;
        cin.get();
    }
}
