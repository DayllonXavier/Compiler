#ifndef LR0Generator_hpp
#define LR0Generator_hpp

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <assert.h>

#include "Rule.hpp"
#include "State.hpp"

using namespace std;

class LR0Generator {
    public:
        LR0Generator();
        ~LR0Generator();
        LR0Generator(string path_to_grammar);

        int nextAvailableID;
        void read_grammar_file(string path_to_grammar);
        void print_rules();
        void close_rule(State& state);
        void add_new_states(int state_number);
        int find_equal_state(State state);

        vector<Rule> rules;
        vector<State> states;

        set<string> terminal;
        set<string> non_terminal;

        void calculate_terminal();
        void calculate_non_terminal();

        map<string,set<string>> first;
        void calculate_first();
        map<string,set<string>> follow;
        void calculate_follow();

        map<int,map<string,string>> SLRTABLE;
        void calculate_slrtable();

        string path_to_write_output = "./";

        void write_action();
        void write_goto();
        void write_terminal();
        void write_non_terminal();
        void write_sizes();
        void write_rule_number_size();
        void write_follow();

        friend ostream& operator<<(ostream& os, const LR0Generator& lr0generator);
};
#endif
