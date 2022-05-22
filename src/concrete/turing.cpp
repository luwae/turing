#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "parse.hpp"

using std::cout; using std::endl;
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

    cout << tm << endl;

    Tape t(argv[2]);
    tm.print(t);
    cout << endl;
    while (!tm.isDone()) {
        tm.step(t);
        tm.print(t);
        cout << endl;
    }
}
