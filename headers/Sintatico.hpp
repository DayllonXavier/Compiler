#ifndef Sintatico_hpp
#define Sintatico_hpp

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <assert.h>
#include <vector>

#include "Token.hpp"
#include "Lexico.hpp"

using namespace std;

class Sintatico
{
    public:
        Sintatico(Lexico& _analisadorLexico);
        ~Sintatico();
    private:
        void initSintatico();
        void loadBasicInformations();
        void loadRules();
        void loadRulesSize();
        void loadTerminalSymbols();
        void loadNonTerminalSymbols();
        void loadActionTable();
        void loadGotoTable();

        const string pathToBasicInformation = "config/sintatico/qtd.txt";
        const string pathToGrammar = "config/sintatico/Grammar/grammar.txt";
        const string pathToRulesSize = "config/sintatico/rule_size.txt";
        const string pathToTerminalSymbols = "config/sintatico/terminalList.txt";
        const string pathToNonTerminalSymbols = "config/sintatico/naoTerminalList.txt";
        const string pathToActionTable = "config/sintatico/action.txt";
        const string pathToGotoTable = "config/sintatico/goto.txt";

        int nbEstados, nbTerminalSymbols, nbNonTerminalSymbols, nbRules;
        Lexico& analisadorLexico;
        map<int, string> rules;
        map<int, int> rulesSize;
        map<string, int> terminalSymbolsIdx, nonTerminalSymbolsIdx;
        vector<vector<int>> gotoTable;
        vector<vector<pair<char, int>>> actionTable;
};

#endif //Sintatico_hpp
