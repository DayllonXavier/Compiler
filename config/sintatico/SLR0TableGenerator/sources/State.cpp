#include "State.hpp"

State::State()
{
}
State::State(int _number)
{
    number = _number;
    reduce_rule = -1;
}

State::~State()
{
}
void State::addRule(Rule rule)
{
    rules.push_back(rule);
    if(rule.isReduction())
    {
        if(reduce())
        {
            cout << "ERROR: CONFLICT REDUCE " << reduce_rule << " REDUCE " << rule.number << " INSIDE STATE " << number << endl;
            exit(1);
        }
        reduce_rule = rule.number;
    }
}
void State::addClosure(string element)
{
    closed.push_back(element);
}
void State::addTransition(string element, int state)
{
     transitions.push_back(make_pair(element, state));

}
bool State::isClosed(string element)
{
    for(auto closed_element : closed)
    {
        if(closed_element == element) return true;
    }
    return false;
}
bool State::isEqual(State& otherState)
{
    if(otherState.rules.size() != rules.size()) return false;
    for(auto otherRule : otherState.rules)
    {
        bool found = false;
        for(auto rule : rules)
        {
            if(rule.isEqual(otherRule))
            {
                found = true;
                break;
            }
        }
        if(found == false) return false;
    }
    return true;
}
bool State::reduce()
{
    return reduce_rule > -1;
}
ostream& operator<<(ostream& os, const State& state)
{
    os << "State " << state.number << endl;
    os << "Depth " << state.depth << endl;
    os << "Rules: " << endl;
    for(auto rule : state.rules)
    {
        os << rule << endl;
    }
    os << "Closures: " << endl;
    for(auto closed : state.closed)
    {
        os << closed << endl;
    }
    os << "Transitions: " << endl;
    for(auto transition : state.transitions)
    {
           os << "(" << transition.first << "," << transition.second << ")" << endl;
    }
    return os;
}
