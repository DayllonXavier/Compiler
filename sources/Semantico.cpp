#include "Semantico.hpp"

Semantico::Semantico(SymbolTable &_symbolTable, string _pathToObjectFile) 
: symbolTable(_symbolTable), pathToObjectFile(_pathToObjectFile)
{
    objectFile.open(pathToObjectFile);
    assert(objectFile.is_open());
    build();
    lastRule = -1;
}

Semantico::~Semantico()
{
    objectFile << "}";
    objectFile.close();
}

void Semantico::build()
{
    objectFile << "#include<stdio.h>\nint main(){\n";
}

void Semantico::stackAdd(Token token)
{
    semanticStack.push(token);
}

void Semantico::stackPop(int nbOfPop)
{
    while (nbOfPop-- > 0)
    {
        semanticStack.pop();
    }
}

Token Semantico::stackTop()
{
    return semanticStack.top();
}

void Semantico::stackClear()
{
    while (!semanticStack.empty())
    {
        semanticStack.pop();
    }
}

void Semantico::popToBuffer(int nbOfPop)
{
    buffer.clear();
    while (nbOfPop-- > 0)
    {
        buffer.push_back(semanticStack.top());
        semanticStack.pop();
    }
    reverse(buffer.begin(), buffer.end());
}

void Semantico::printBuffer(){
    cout << "--> ";
    for (Token token : buffer)
        cout << token << "   ";
    cout << endl;
}

void Semantico::setReadPos(pair<int, int> _readPos)
{
    readPos = _readPos;
}

bool Semantico::process(int idxOfRule, int sizeOfRule)
{
    popToBuffer(sizeOfRule);
    if (!executeRule(idxOfRule))
    {
        cout << "ERRO SEMANTICO -> (Linha: " << readPos.first << " Coluna: " << readPos.second << ")" << endl;
        return false;
    }
    printBuffer();
    lastRule = idxOfRule;
    return true;
}

bool Semantico::executeRule(int idxOfRule){
    bool ok = false;
    switch (idxOfRule)
    {
    case 5:
        ok = regra_5();
        break;
    case 6:
        ok = regra_6();
        break;
    case 7:
        ok = regra_7();
        break;
    case 8:
        ok = regra_8();
        break;
    case 9:
        ok = regra_9_10_11();
        break;
    case 10:
        ok = regra_9_10_11();
        break;
    case 11:
        ok = regra_9_10_11();
        break;
    }
    return ok;
}

void Semantico::printObjectFile(string s){
    objectFile << s;
}

bool Semantico::regra_5()
{
    printObjectFile("\n\n\n");
    return true;
}

bool Semantico::regra_6()
{
    printObjectFile(";\n");
    semanticStack.push(Token("_", "D", lastType));
    return true;
}

bool Semantico::regra_7()
{
    if (buffer[0].getTipo() != "NULO")
        return false;
    buffer[0].setTipo(lastType);
    symbolTable.atualizar(buffer[0]);
    semanticStack.push(Token("_", "L", lastType));
    printObjectFile(",");
    printObjectFile(buffer[0].getLexema());
    return true;
}

bool Semantico::regra_8(){
    if (buffer[0].getTipo() != "NULO")
        return false;
    buffer[0].setTipo(lastType);
    symbolTable.atualizar(buffer[0]);
    semanticStack.push(buffer[0]);
    printObjectFile(buffer[0].getLexema());
    return true;
}

bool Semantico::regra_9_10_11()
{
    Token token("_", "TIPO", buffer[0].getTipo());
    semanticStack.push(token);
    printObjectFile(token.getTipo()+" ");
    lastType = token.getTipo();
    return true;
}