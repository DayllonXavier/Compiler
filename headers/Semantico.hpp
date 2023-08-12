#ifndef Semantico_hpp
#define Semantico_hpp

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <fstream>

#include "Token.hpp"
#include "SymbolTable.hpp"

using namespace std;

class Semantico
{
public:
    Semantico(SymbolTable &_symbolTable, string _pathToObjectFile);
    ~Semantico();
    void printBuffer();
    void stackAdd(Token token);
    void setReadPos(pair<int, int> _readPos);
    bool process(int idxOfRule, int sizeOfRule);

private:
    void build();
    void popToBuffer(int nbOfPop = 1);
    void stackPop(int nbOfPop = 1);
    Token stackTop();
    void stackClear();
    bool executeRule(int idxOfRule);
    void printObjectFile(string s);

    SymbolTable &symbolTable;
    string pathToObjectFile;
    stack<Token> semanticStack;
    vector<Token> buffer;
    pair<int, int> readPos;
    ofstream objectFile;
    string lastType;
    int lastRule;

protected:
    bool regra_5();
    bool regra_6();
    bool regra_7();
    bool regra_8();
    bool regra_9_10_11();
};

#endif // Semantico_hpp
