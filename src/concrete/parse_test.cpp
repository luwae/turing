#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "parse.hpp"

using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using parse::Parser;
using std::string;

int main() {
    ifstream file("../../machines/concrete.atm");
    stringstream ss;
    ss << file.rdbuf();
    string input = ss.str();

    cout << "file contents:\n";
    cout << input << endl;
   
    TuringMachine tm;
    Parser ps(input, tm);
    cout << "finished parsing" << endl;

    cout << tm << endl;

    Tape t("101011");
    tm.print(t);
    cout << endl;
    while (!tm.isDone()) {
        tm.step(t);
        tm.print(t);
        cout << endl;
    }
}
