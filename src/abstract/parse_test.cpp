#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <set>

#include "parse.hpp"

using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using parse::Parser;
using std::string;
using std::unique_ptr; using std::make_unique;
using std::set;

int main() {
    ifstream file("../../machines/2.atm");
    stringstream ss;
    ss << file.rdbuf();
    string input = ss.str();

    cout << "file contents:\n";
    cout << input << endl;
   
    Parser ps(input);
    cout << "finished parsing" << endl;

    for (const auto &s: ps.states)
        cout << s << "\n";
    cout << "\n\n\n" << endl;
    
    State maincopy = ps.states[0];
    set<string> rnames;
    maincopy.expand(cout, ps.states, rnames);
}
