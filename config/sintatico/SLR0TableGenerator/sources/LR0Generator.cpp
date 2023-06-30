#include <iostream>
#include <fstream>
#include "LR0Generator.hpp"

LR0Generator::LR0Generator()
{
}
LR0Generator::~LR0Generator()
{
}
LR0Generator::LR0Generator(string path_to_grammar)
{
    read_grammar_file(path_to_grammar);

    State initial_state(0);
    initial_state.addRule(rules.front().addDot());
    close_rule(initial_state);
    initial_state.depth = 0;
    states.push_back(initial_state);
    nextAvailableID = 1;

    for(int i=0; i<states.size(); i++)
    {
        //cout << "PROCESSING STATE " << i << endl;
        //cout << states[i];
        add_new_states(i);
        //cout << "FINISHED STATE " << i << endl;
    }
}
int LR0Generator::find_equal_state(State state)
{
    for(int i=0; i< states.size(); i++)
    {
        if(states[i].isEqual(state)) return i;
    }
    return -1;
}
void LR0Generator::add_new_states(int state_number)
{
    //cout << "Trying to add new states from " << states[state_number].number << endl;
    //cout << states[state_number] << endl;
    for(auto element : states[state_number].closed)
    {
        //cout << "Adding new states from " << states[state_number].number << " to " << element << endl;
        State new_state(-1);
        for(auto rule : states[state_number].rules)
        {
            if(rule.getDotElement() == element)
            {
                //cout << rule.getDotElement() << " == " << element << endl;
                //cout << "adding " << rule.nextDot() << endl;
                new_state.addRule(rule.nextDot());
            }
        }
        close_rule(new_state);
        int already_exists = find_equal_state(new_state);
        if(already_exists != -1)
        {
            //cout << "State already exists\n";
            states[state_number].addTransition(element,already_exists);
        }
        else
        {
            new_state.number = nextAvailableID;
            nextAvailableID++;
            //cout << "Adding new state " << new_state.number << endl;
            //cout << new_state << endl;
            states[state_number].addTransition(element,new_state.number);
            new_state.depth = states[state_number].depth + 1;
            states.push_back(new_state);
        }
    }
    //cout << "Finished State " << states[state_number].number << endl;
}
void LR0Generator::read_grammar_file(string path_to_grammar)
{
    ifstream file(path_to_grammar);
    assert(file.is_open());
    int number;
    string left, arrow, right;
    while( file >> number >> left >> arrow)
    {
        file.ignore();
        if(!getline(file,right))break;
        rules.push_back(Rule(number,left,right));
    }
    file.close();
}
void LR0Generator::close_rule(State& state)
{
    //cout << "Closing state " << state.number << endl;
    for(int i = 0; i < state.rules.size(); i++)
    {
        Rule & rule = state.rules[i];
        //cout << "Analyzing rule " << rule << endl;
        string dot = rule.getDotElement();
        //cout << "dot = " << dot;
        if(dot == "") continue;
        if(state.isClosed(dot))
        {
            //cout << dot << " is already closed\n";
            continue;
        }
        for(auto scan_rule : rules)
        {
            if(dot == scan_rule.left)
            {
                state.addRule(scan_rule.addDot());
            }
        }
        state.addClosure(dot);
        //cout << "adding " << dot << " closure\n";
    }
    //cout << "Closed state " << state.number << endl;
}
void LR0Generator::print_rules()
{
    cout << "RULES:\n";
    for(auto rule : rules)
    {
        cout << rule << endl;
    }
}
ostream& operator<<(ostream& os, const LR0Generator& lr0generator)
{
    os << "States:\n";
    for(auto state : lr0generator.states)
    {
        os << state << endl;
    }
    return os;
}

void LR0Generator::calculate_non_terminal()
{
    for(auto rule : rules)
    {
        non_terminal.insert(rule.left);
    }
}

void LR0Generator::calculate_terminal()
{
    terminal.insert("EOF");

    for(auto rule : rules)
    {
        rule = rule.addDot();
        string val = rule.getDotElement();
        while(val != "")
        {
            if(non_terminal.find(val) == non_terminal.end())
            {
                terminal.insert(val);
            }
            rule = rule.nextDot();
            val = rule.getDotElement();
        }
    }
}

void LR0Generator::calculate_first()
{
    //adding trivial terminal first
    for(auto t : terminal)
    {
        first[t].insert(t);
    }

    //adding first
    for(auto rule : rules)
    {
        rule = rule.addDot();
        string val = rule.getDotElement();
        first[rule.left].insert(val);
    }

    //removing E from F(E) so its not recursive
    for(auto nt : non_terminal)
    {
        first[nt].erase(nt);
    }

    //sweeping non terminals one by one solving each dependency
    for(auto nt : non_terminal)
    {
        for(auto & f : first)
        {
            if(f.second.find(nt) != f.second.end())
            {
                f.second.erase(nt);
                for(auto nt_f : first[nt])
                {
                    f.second.insert(nt_f);
                }
            }
        }
    }
}

void LR0Generator::calculate_follow()
{
    follow[rules[0].left].insert("EOF");

    for(auto rule : rules)
    {
        rule = rule.addDot();
        string fw = rule.getDotElement();
        while(fw != "")
        {
            if(non_terminal.find(fw) != non_terminal.end())
            {
                string next = rule.nextDot().getDotElement();
                if(next == "")
                {
                    follow[fw].insert(rule.left);
                }else{
                    for(auto f : first[next])
                    {
                        follow[fw].insert(f);
                    }
                }
            }

            rule = rule.nextDot();
            fw = rule.getDotElement();
        }
    }

    //sweeping non terminals one by one solving each dependency
    for(auto nt : non_terminal)
    {
        for(auto & fw : follow)
        {
            if(fw.second.find(nt) != fw.second.end())
            {
                fw.second.erase(nt);
                for(auto nt_f : follow[nt])
                {
                    fw.second.insert(nt_f);
                }
            }
        }
    }
}

void LR0Generator::add_to_slrtable(int state_number, string token, string value)
{
     if(value == "A 0")
     {
         cout << "SOFT WARNING:TABLE[" << state_number << "][" << token << "] ALREADY HAS A VALUE, ";
         cout << "VALUE = " << SLRTABLE[state_number][token] << " TRYING TO ADD " << value << endl;
     }
     else if(SLRTABLE[state_number][token] != "")
     {
         cout << "ERROR: TABLE[" << state_number << "][" << token << "] ALREADY HAS A VALUE, ";
         cout << "VALUE = " << SLRTABLE[state_number][token] << " TRYING TO ADD " << value << endl;
         exit(2);
     }

     SLRTABLE[state_number][token] = value;
}

void LR0Generator::calculate_slrtable()
{
     for(auto state : states)
     {
         if(state.reduce())
         {
             Rule r;
             for(auto rule : rules)
             {
                 if(rule.number == state.reduce_rule)
                 {
                     r = rule;
                     break;
                 }
             }

             for(auto t : follow[r.left])
             {
                 add_to_slrtable(state.number, t, string("R ").append(to_string(state.reduce_rule)));
             }

             if(r.number == 1)
             {
                 add_to_slrtable(state.number, "EOF", "A 0");
             }
         }

         for(auto transition : state.transitions)
         {
             if(terminal.find(transition.first) != terminal.end())
             {
                 add_to_slrtable(state.number, transition.first, string("S ").append(to_string(transition.second))); 
             }else{
                 add_to_slrtable(state.number, transition.first, to_string(transition.second));
             }
         }


     }
}

void LR0Generator::write_terminal()
{
    ofstream f;
    string output_path = path_to_write_output;
    f.open(output_path.append("terminalList.txt"));
    for(auto t : terminal)
    {
        f << t << endl;
    }
    f.close();
}
void LR0Generator::write_non_terminal()
{
    ofstream f;
    string output_path = path_to_write_output;
    f.open(output_path.append("naoTerminalList.txt"));
    for(auto nt : non_terminal)
    {
        f << nt << endl;
    }
    f.close();
}

void LR0Generator::write_action()
{
    ofstream f;
    string output_path = path_to_write_output;
    f.open(output_path.append("action.txt"));

    //cout << "       ";
    //for(auto t : terminal)
    //{
    //    cout << t << "    ";

    //}
    //cout << endl;
    for(auto state : SLRTABLE)
    {
        //cout << "STATE" << state.first << " ";
        for(auto t : terminal)
        {
            if(SLRTABLE[state.first][t].empty())
            {
                f << "E 0 ";
            }else{
                f << SLRTABLE[state.first][t] << " ";
            }
        }
        f << endl;
    }
    f.close();
}

void LR0Generator::write_goto()
{
    ofstream f;
    string output_path = path_to_write_output;
    f.open(output_path.append("goto.txt"));

    for(auto state : SLRTABLE)
    {
        //cout << "STATE" << state.first << " ";
        for(auto nt : non_terminal)
        {
            if(SLRTABLE[state.first][nt].empty())
            {
                f << "-1 ";
            }else{
                f << SLRTABLE[state.first][nt] << " ";
            }
        }
        f << endl;
    }
    f.close();
}

void LR0Generator::write_sizes()
{
    ofstream f;
    string output_path = path_to_write_output;
    f.open(output_path.append("qtd.txt"));

    f << states.size() << endl;
    f << terminal.size() << endl;
    f << non_terminal.size() << endl;
    f << rules.size() << endl;

    f.close();
}

void LR0Generator::write_rule_number_size()
{
    ofstream f;
    string output_path = path_to_write_output;
    f.open(output_path.append("rule_size.txt"));

    for(auto rule : rules)
    {
        int size = 0;
        Rule r = rule.addDot();
        while(r.getDotElement() != "")
        {
            size++;
            r = r.nextDot();
        }
        f << rule.number << " " << size << endl;
    }
    f.close();
}

void LR0Generator::write_follow()
{
    ofstream f;
    string output_path = path_to_write_output;
    f.open(output_path.append("follow.txt"));

    for(auto nt : non_terminal)
    {
        for(auto t : terminal)
        {
            if(follow[nt].find(t) != follow[nt].end())
            {
                f << "1 ";
            }else{
                f << "0 ";
            }
        }
        f << endl;
    }

    f.close();
}
