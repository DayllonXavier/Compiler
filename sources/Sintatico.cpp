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
    string leftOfRule, rule;
    for (int i = 0; i < nbRules; i++){
        file >> id >> leftOfRule;
        getline(file, rule);
        leftOfRules[id] = leftOfRule;
        rules[id] = leftOfRule + " " + rule;
        //cout << id<<  "  --(" << leftOfRule << ")---   " << rules[id] << endl;
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

void Sintatico::stackAdd(int value)
{
    automatonStack.push(value);
    //cout << "add " << value << endl;
}

void Sintatico::stackPop(int nbOfPop)
{
    //cout << "A" << endl;
    while(nbOfPop-- > 0){
        //cout << "pop ";
        //cout << automatonStack.top() << endl;
        automatonStack.pop();
    }
}

int Sintatico::stackTop()
{
    return automatonStack.top();
}

void Sintatico::stackClear(){
    while (!automatonStack.empty()){
        automatonStack.pop();
    }
}

void Sintatico::getNextToken()
{
    token = analisadorLexico.SCANNER();
    while(token.getClasse() == "ERRO"){
        token = analisadorLexico.SCANNER();
    }
    //cout << state << " " << token.getClasse() << " " << terminalSymbolsIdx[token.getClasse()] << endl;
    //int a;
    //cin >> a;
}

void Sintatico::printRule(int idxOfRule)
{
    cout << rules[idxOfRule] << endl;
}

bool Sintatico::process()
{
    stackClear();
    stackAdd(0);
    getNextToken();
    pair<char, int> movement;
    char actionType;
    int actionMove;
    int stateT;

    while(true){
        state = stackTop();
        movement = actionTable[state][terminalSymbolsIdx[token.getClasse()]];
        actionType = movement.first;
        actionMove = movement.second;
        
        cout << state << " " << token.getClasse() << " " << terminalSymbolsIdx[token.getClasse()] << endl;
        if (actionType == 'S'){
            stackAdd(actionMove);
            getNextToken();
        }
        else if (actionType == 'R'){
            stackPop(rulesSize[actionMove]);
            stateT = stackTop();
            stackAdd(gotoTable[stateT][nonTerminalSymbolsIdx[leftOfRules[actionMove]]]);
            printRule(actionMove);
        }
        else if (actionType == 'A'){
            return true;
        }
        else{
            fixError();
        }
    }

    return false;
}

void Sintatico::fixError()
{
    cout << "SINTATICAL ERROR FOUNDED" << endl;
    int a;
    cin >> a;
}