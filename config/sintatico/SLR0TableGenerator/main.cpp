#include <iostream>
#include <string>
#include "LR0Generator.hpp"
#include "XMLGenerator.hpp"

using namespace std;

int main(int argc, char *argv[])
{

    //string path_to_grammar = argv[1];
    //LR0Generator gen(path_to_grammar);
    //gen.print();
    //Rule r(1,"E'", "E");
    //cout << r.addDot() << endl;
    //State state(0);
    //state.addRule(r);
    //state.addClosure("P");
    //state.addTransition("+", 1);
    //cout << state << endl;

    string path_to_grammar = argv[1];
    LR0Generator gen(path_to_grammar);

    //gen.print_rules();
    cout << gen << endl;
    //return 0;

    gen.calculate_non_terminal();
    gen.calculate_terminal();
    cout << "Non terminal = "<< endl;
    for(auto value : gen.non_terminal)
    {
        cout << value << endl;
    }

    cout << "Terminal = "<< endl;
    for(auto value : gen.terminal)
    {
        cout << value << endl;
    }

    gen.calculate_first();
    cout << "First: " << endl;
    for(auto e : gen.first)
    {
        cout << "F(" << e.first << ") = { ";
        for(auto value : e.second)
        {
            cout << value << " ";
        }
        cout << "}" << endl;
    }

    gen.calculate_follow();
    cout << "Follow: " << endl;
    for(auto e : gen.follow)
    {
        cout << "FW(" << e.first << ") = { ";
        for(auto value : e.second)
        {
            cout << value << " ";
        }
        cout << "}" << endl;
    }

    gen.calculate_slrtable();
    cout << "slrtable: " << endl;
    for(auto state : gen.SLRTABLE)
    {
        cout << "State " << state.first << endl;
        for(auto value : state.second)
        {
            cout << "(" << value.first << ", " << value.second << ") ";
        }
        cout << endl;
    }

    gen.write_action();
    gen.write_goto();
    gen.write_terminal();
    gen.write_non_terminal();
    gen.write_sizes();
    gen.write_rule_number_size();
    gen.write_follow();

    //XMLGenerator xml(gen);
    //xml.print_header();
    //xml.recursive_draw(xml.lr0generator.states[0],0,0,false, -1, 0);
    //xml.print_footer();
    //return 0;
}
