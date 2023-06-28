#include "Sintatico.hpp"

Sintatico::Sintatico(Lexico& _analisadorLexico)
: analisadorLexico(_analisadorLexico)
{
    initSintatico();
}

Sintatico::~Sintatico()
{
}

void Sintatico::initSintatico()
{
    loadBasicInformations();
    loadRules();
    loadRulesSize();
    loadTerminalSymbols();
    loadNonTerminalSymbols();
    loadActionTable();
    loadGotoTable();
}

void Sintatico::loadBasicInformations()
{
    ifstream file(pathToBasicInformation);
    assert(file.is_open());
    file >> nbEstados;
    file >> nbTerminalSymbols;
    file >> nbNonTerminalSymbols;
    file >> nbRules;
    file.close();
    //cout << nbEstados << " " << nbTerminalSymbols << " " << nbNonTerminalSymbols << " " << nbRules << endl;
}

void Sintatico::loadRules()
{
    ifstream file(pathToGrammar);
    assert(file.is_open());
    int id;
    string rule;
    for (int i = 0; i < nbRules; i++){
        file >> id;
        getline(file, rule);
        //cout << id<<  "  -----   " << rule << endl;
        rules[id] = rule;
    }
    file.close();
}

void Sintatico::loadRulesSize()
{
    ifstream file(pathToRulesSize);
    assert(file.is_open());
    int id;
    int ruleSize;
    for (int i = 0; i < nbRules; i++)
    {
        file >> id >> ruleSize;
        //cout << id <<  "  -----   " << ruleSize << endl;
        rulesSize[id] = ruleSize;
    }
    file.close();
}

void Sintatico::loadTerminalSymbols()
{
    ifstream file(pathToTerminalSymbols);
    assert(file.is_open());
    string terminalSymbol;
    for (int i = 0; i < nbTerminalSymbols; i++){
        file >> terminalSymbol;
        terminalSymbolsIdx[terminalSymbol] = i;
        //cout << terminalSymbol << " " << terminalSymbolsIdx[terminalSymbol] << endl;
    }
    file.close();
}

void Sintatico::loadNonTerminalSymbols()
{
    ifstream file(pathToNonTerminalSymbols);
    assert(file.is_open());
    string nonTerminalSymbol;
    for (int i = 0; i < nbNonTerminalSymbols; i++)
    {
        file >> nonTerminalSymbol;
        nonTerminalSymbolsIdx[nonTerminalSymbol] = i;
        //cout << nonTerminalSymbol << " " << nonTerminalSymbolsIdx[nonTerminalSymbol] << endl;
    }
    file.close();
}

void Sintatico::loadActionTable()
{
    ifstream file(pathToActionTable);
    assert(file.is_open());

    actionTable.clear();
    actionTable.resize(nbEstados, vector<pair<char, int>>(nbTerminalSymbols));

    char actionType;
    int actionMove;

    for (int i = 0; i < nbEstados; i++){
        for (int j = 0; j < nbTerminalSymbols; j++){
            file >> actionType >> actionMove;
            actionTable[i][j] = {actionType, actionMove};
        }
    }

    /*for (int i = 0; i < nbEstados; i++)
    {
        for (int j = 0; j < nbTerminalSymbols; j++)
        {
            cout << "(" << actionTable[i][j].first << ", " << actionTable[i][j].second << ") ";
        }
        cout << endl;
    }*/

    file.close();
}

void Sintatico::loadGotoTable()
{
    ifstream file(pathToGotoTable);
    assert(file.is_open());

    gotoTable.clear();
    gotoTable.resize(nbEstados, vector<int>(nbNonTerminalSymbols));

    for (int i = 0; i < nbEstados; i++){
        for (int j = 0; j < nbNonTerminalSymbols; j++){
            file >> gotoTable[i][j];
        }
    }

    /*for (int i = 0; i < nbEstados; i++)
    {
        for (int j = 0; j < nbNonTerminalSymbols; j++)
        {
            cout << gotoTable[i][j] << " ";
        }
        cout << endl;
    }*/

    file.close();
}
