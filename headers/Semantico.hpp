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
    void finalize(bool ok);
    void generateFinalFile();

private:
    void head();
    void footer();
    void popToBuffer(int nbOfPop = 1);
    void stackPop(int nbOfPop = 1);
    Token stackTop();
    void stackClear();
    bool executeRule(int idxOfRule);
    void printErrorMessage(string message);
    void printObjectFile(string s);
    void eraseBytes(int nbOfBytesToErase);
    string convertTypes(string s);

    SymbolTable &symbolTable;
    string pathToObjectFile;
    string pathToTempObjectFile = "./config/bin/out.c";
    stack<Token> semanticStack;
    vector<Token> buffer;
    pair<int, int> readPos;
    ofstream objectFile;
    string lastType, lastExpr;
    vector<pair<string, string>> tempVars;
    int nbOpenBrackets, nbBytesPrintedLast;
    bool newLine;

protected:
    bool regra_1();
    bool regra_2();
    bool regra_3();
    bool regra_4();
    bool regra_5();
    bool regra_6();
    bool regra_7();
    bool regra_8();
    bool regra_9_10_11();
    bool regra_12_24_32_39();
    bool regra_13();
    bool regra_14();
    bool regra_15_16_21_23();
    bool regra_17();
    bool regra_18();
    bool regra_19();
    bool regra_20();
    bool regra_22();
    bool regra_25();
    bool regra_26();
    bool regra_27();
    bool regra_28_29_30_31();
    bool regra_33();
    bool regra_34();
    bool regra_35_36_37_38();
};

#endif // Semantico_hpp
