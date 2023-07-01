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
    loadErrorProductionMessages();
    readPos = {0, 0};
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

void Sintatico::loadErrorProductionMessages()
{
    int nbErrorProductionMessages;
    string nonTerminalSymbol, message;

    ifstream file(pathToErrorProductionMessages);
    assert(file.is_open());

    errorProductionMessages.clear();

    file >> nbErrorProductionMessages;
    for (int i = 0; i < nbErrorProductionMessages; i++)
    {
        file >> nonTerminalSymbol;
        getline(file, message);
        errorProductionMessages[nonTerminalSymbol] = message.substr(1, -1);
        //cout << nonTerminalSymbol << " " << errorProductionMessages[nonTerminalSymbol] << endl;
    }

    file.close();
}

void Sintatico::stackAdd(int value)
{
    automatonStack.push(value);
    infoAutomatonStack.push(readPos);
    //cout << "add " << value << endl;
}

void Sintatico::stackPop(int nbOfPop)
{
    //cout << "A" << endl;
    while(nbOfPop-- > 0){
        //cout << "pop ";
        //cout << automatonStack.top() << endl;
        automatonStack.pop();
        infoAutomatonStack.pop();
    }
}

int Sintatico::stackTop()
{
    return automatonStack.top();
}

void Sintatico::stackClear(){
    while (!automatonStack.empty()){
        automatonStack.pop();
        infoAutomatonStack.pop();
    }
}

void Sintatico::getNextToken()
{
    token = analisadorLexico.SCANNER();
    while(token.getClasse() == "ERRO"){
        token = analisadorLexico.SCANNER();
    }
    readPos = analisadorLexico.getPos();
    //cout << state << " " << token.getClasse() << " " << terminalSymbolsIdx[token.getClasse()] << " -- " << analisadorLexico.getPos().first << " " << analisadorLexico.getPos().second << endl;
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
        
        //cout << state << " " << token.getClasse() << " " << terminalSymbolsIdx[token.getClasse()] << endl;
        if (actionType == 'S'){
            stackAdd(actionMove);
            getNextToken();
        }
        else if (actionType == 'R'){
            stackPop(rulesSize[actionMove]);
            fixErrorProductions(actionMove);
            printRule(actionMove);
            stateT = stackTop();
            stackAdd(gotoTable[stateT][nonTerminalSymbolsIdx[leftOfRules[actionMove]]]);
        }
        else if (actionType == 'A'){
            return true;
        }
        else{
            if(token.getClasse() == "eof") break;// No more tokens to discard, exit
            fixError();
        }
    }

    return false;
}

void Sintatico::fixError()
{
    cout << "SYNTAX ERROR FOUND on line " << infoAutomatonStack.top().first << " and column " << infoAutomatonStack.top().second << endl;

    cout << "FIXING USING PANIC MODE\n";

    pair<char, int> movement;
    char actionType;
    int actionMove;
    int stateT;

    // Finding Syncronizing Token
    while(token.getClasse() != "pt_v" && token.getClasse() != "fc_p" && token.getClasse() != "eof")
    {
        cout << "Ignoring token " << token.getClasse() << " to fix error \n";
        getNextToken();
    }

    // Skip to next token after syncronizing token
    getNextToken();

    // Discard states inside stack until a valid transition is found
    while(true){
        state = stackTop();
        movement = actionTable[state][terminalSymbolsIdx[token.getClasse()]];
        actionType = movement.first;

        // Edge case: stack cannot be empty, if(size == 1) exit
        if (actionType != 'E' || automatonStack.size() == 1){
            break;
        }else{
            stackPop();
        }
    }
    cout << "END OF PANIC MODE!" << endl;
}

void Sintatico::fixErrorProductions(int ruleIdx)
{
    if (errorProductionMessages.count(leftOfRules[ruleIdx])){
        cout << "SYNTAX ERROR FOUND on line " << infoAutomatonStack.top().first << " and column " << infoAutomatonStack.top().second << endl;
        cout << "\t" << errorProductionMessages[leftOfRules[ruleIdx]] << endl; 
    }
}
