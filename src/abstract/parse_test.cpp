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
    ifstream file("../../machines/simple.atm");
    stringstream ss;
    ss << file.rdbuf();
    string input = ss.str();

    cout << "file contents:\n";
    cout << input << endl;
   
    Parser ps(input);
    cout << "finished parsing" << endl;

    for (const auto &s: ps.states)
        cout << s << endl;

    State p(ps.states[2]);
    State fr(ps.states[3]);
    p.apply_chr('?');
    cout << p << endl;
    p.apply_state("XXX");
    cout << p << endl;
    fr.apply_chr('?');
    cout << fr << endl;
    fr.apply_state("XXX");
    cout << fr << endl;

    for (const auto &s: ps.states)
        cout << s << endl;
}
