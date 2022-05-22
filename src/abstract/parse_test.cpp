#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "parse.hpp"

using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using parse::Parser; using parse::State;
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
}
