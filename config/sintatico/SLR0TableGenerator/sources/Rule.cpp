#include "Rule.hpp"

Rule::Rule()
{
}
Rule::~Rule()
{
}
Rule::Rule(int _number, string _left, string _right)
{
    number = _number;
    left = _left;
    right = _right;
}
string Rule::getDotElement()
{
    int dot = right.find(".");
    if(dot == -1) return "";
    string after_dot = right.substr(dot+1);
    int space = after_dot.find(" ");
    string before_space = after_dot.substr(0,space);
    return before_space;
}

Rule Rule::addDot()
{
    if(getDotElement() != "") return Rule(number,left,right);

    string dot = ".";
    return Rule(number, left, dot.append(right));
}

Rule Rule::nextDot()
{

    int dot = right.find(".");
    int space = right.substr(dot).find(" "); 
    if(space == -1) space = right.size()-1;
    else space += dot;
    string left_of_dot = right.substr(0,dot);
    string word_of_dot = right.substr(dot+1,space+1-(dot+1));//This imposes a restriction where the string cannot have more than one space
    string right_of_dot = right.substr(space+1);
    string newRight = left_of_dot.append(word_of_dot).append(".").append(right_of_dot);

    return Rule(number,left,newRight);
}
bool Rule::isEqual(Rule otherRule)
{
    if(number != otherRule.number) return false;
    if(right.find(".") != otherRule.right.find(".")) return false;
    return true;
}
bool Rule::isReduction()
{
    return getDotElement() == "";
}

ostream& operator<<(ostream& os, const Rule& rule)
{
    return os << "(" << rule.number << ") " << rule.left << " -> " << rule.right;
}
