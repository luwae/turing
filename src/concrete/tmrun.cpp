#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "machine.hpp"
#include "tmlang/parse.hpp"

using std::cout; using std::endl;
using std::ifstream;
using std::stringstream;
using parse::Parser;
using std::string;

bool file_extension(const string &name, const string &ext) {
    if (name.length() >= ext.length()) {
        return name.compare(name.length() - ext.length(), ext.length(), ext) == 0;
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "usage: tmrun <machine file> [<tape input>]" << endl;
        return -1;
    }
    
    if (!file_extension(argv[1], ".tm")) {
    	cout << "error: only supports .tm machine files" << endl;
    	return -1;
    }
    
    ifstream in(argv[1]);
    stringstream buffer;
    buffer << in.rdbuf();

    TuringMachine tm;
    Parser parser(buffer.str(), tm);


    MachineExecution me(&tm, (argc >= 3) ? argv[2] : "");
    while (me.get_term() == TerminateType::term_cont) {
        me.step();
        cout << me << endl;
    }
    
    static string termstate[] = {"continue", "accept", "reject", "fail"};
    cout << "machine finished with exit state " << termstate[me.get_term()] << endl;
}
