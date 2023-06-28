#ifndef Rule_hpp
#define Rule_hpp

#include <string>
#include <iostream>

using namespace std;

class Rule {
    public:
        int number;
        string left;
        string right;

        Rule();
        ~Rule();
        Rule(int number, string left, string right);
        string getDotElement();
        Rule addDot();
        Rule nextDot();
        bool isEqual(Rule otherRule);
        bool isReduction();

        friend ostream& operator<<(ostream& os, const Rule& rule);
};

#endif
