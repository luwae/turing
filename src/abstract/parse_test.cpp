#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

#include "parse.hpp"

using std::cout; using std::endl;
using std::ifstream; using std::stringstream;
using parse::Parser;
using std::string;
using std::unique_ptr; using std::make_unique;

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

    unique_ptr<Call> nc = make_unique<Call>();
    nc->type = Call::Type::ct_state_imm;
    nc->name = "p";
    nc->args.emplace_back((unsigned char) '!');

    State fr(ps.states[3]);
    cout << fr << endl;
    fr.apply_chr('?');
    cout << fr << endl;
    fr.apply_state(std::move(nc));
    cout << fr << endl;
    cout << fr.rname() << endl;

    for (const auto &s: ps.states)
        cout << s << endl;
}
