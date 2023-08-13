#include "Semantico.hpp"

Semantico::Semantico(SymbolTable &_symbolTable, string _pathToObjectFile) 
: symbolTable(_symbolTable), pathToObjectFile(_pathToObjectFile)
{
    objectFile.open(pathToTempObjectFile);
    assert(objectFile.is_open());
    nbOpenBrackets = 1;
    newLine = true;
    head();
}

Semantico::~Semantico()
{
    stackClear();
    if (objectFile.is_open())
        objectFile.close();
}

void Semantico::head()
{
    objectFile << "#include<stdio.h>\ntypedef char literal[256];\n\nint main(){\n";
}

void Semantico::footer()
{
    objectFile << "\n\treturn 0;\n}";
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
        return false;
    }
    //printBuffer();
    return true;
}

void Semantico::finalize(bool ok)
{
    footer();
    objectFile.close();
    if (ok)
        generateFinalFile();
}

void Semantico::generateFinalFile()
{
    objectFile.open(pathToObjectFile);
    assert(objectFile.is_open());

    ifstream readFile(pathToTempObjectFile);
    assert(readFile.is_open());

    bool lookinfForFirstBracket = true;
    char ch;
    readFile >> noskipws;
    readFile >> ch;
    do{
        objectFile << ch;
        if (lookinfForFirstBracket && ch == '{'){
            lookinfForFirstBracket = false;
            objectFile << "\n\t/*----Variaveis temporarias----*/\n";
            for (pair<string, string> item : tempVars)
                objectFile << "\t" + convertTypes(item.second) + " " + item.first + ";\n";
            objectFile << "\t/*------------------------------*/\n";
        }
        readFile >> ch;
    } while(!readFile.eof());

    readFile.close();
    objectFile.close();
}

void Semantico::printErrorMessage(string message){
    cout << "ERRO SEMANTICO -> (Linha: " << readPos.first << " Coluna: " << readPos.second << ")" << endl;
    cout << "\t\t" << message << endl;
}

bool Semantico::executeRule(int idxOfRule){
    switch (idxOfRule)
    {
        case 1: return regra_1();
        case 2: return regra_2();
        case 3: return regra_3();
        case 4: return regra_4();
        case 5: return regra_5();
        case 6: return regra_6();
        case 7: return regra_7();
        case 8: return regra_8();
        case 9: return regra_9_10_11();
        case 10: return regra_9_10_11();
        case 11: return regra_9_10_11();
        case 12: return regra_12_24_32_39();
        case 13: return regra_13();
        case 14: return regra_14();
        case 15: return regra_15_16_21_23();
        case 16: return regra_15_16_21_23();
        case 17: return regra_17();
        case 18: return regra_18();
        case 19: return regra_19();
        case 20: return regra_20();
        case 21: return regra_15_16_21_23();
        case 22: return regra_22();
        case 23: return regra_15_16_21_23();
        case 24: return regra_12_24_32_39();
        case 25: return regra_25();
        case 26: return regra_26();
        case 27: return regra_27();
        case 28: return regra_28_29_30_31();
        case 29: return regra_28_29_30_31();
        case 30: return regra_28_29_30_31();
        case 31: return regra_28_29_30_31();
        case 32: return regra_12_24_32_39();
        case 33: return regra_33();
        case 34: return regra_34();
        case 35: return regra_35_36_37_38();
        case 36: return regra_35_36_37_38();
        case 37: return regra_35_36_37_38();
        case 38: return regra_35_36_37_38();
        case 39: return regra_12_24_32_39();
    }
    return false;
}

string Semantico::convertTypes(string s){
    if (s == "literal") return "literal";
    else if (s == "inteiro") return "int";
    else if (s == "real") return "double";
    return "";
}

void Semantico::printObjectFile(string s){
    int fix = 0;
    if (s.back() == '}')
        nbOpenBrackets--;
    if (newLine){
        for (int i = 0; i < nbOpenBrackets; i++)
            objectFile << "\t";
    }
    if (s.back() == '{'){
        nbOpenBrackets++;
        fix = 1;
    }

    newLine = false;
    if (s.back() == ';' || s.back() == '{' || s.back() == '}'){
        s.push_back('\n');
    }
    objectFile << s;
    if (s.back() == '\n')
        newLine = true;
    nbBytesPrintedLast = nbOpenBrackets - fix + s.size();
}

void Semantico::eraseBytes(int nbOfBytesToErase)
{
    objectFile.seekp(-nbOfBytesToErase, ios::cur);
}

bool Semantico::regra_1()
{
    stackAdd(Token("_", "P'", "NULO"));
    return true;
}

bool Semantico::regra_2()
{
    stackAdd(Token("_", "P", "NULO"));
    return true;
}

bool Semantico::regra_3()
{
    stackAdd(Token("_", "V", "NULO"));
    return true;
}

bool Semantico::regra_4()
{
    stackAdd(Token("_", "LV", "NULO"));
    return true;
}

bool Semantico::regra_5()
{
    stackAdd(Token("_", "LV", "NULO"));
    printObjectFile("\n\n\n");
    return true;
}

bool Semantico::regra_6()
{
    stackAdd(Token("_", "D", lastType));
    printObjectFile(";");
    return true;
}

bool Semantico::regra_7()
{
    if (buffer[0].getTipo() != "NULO"){
        printErrorMessage("Variável já declarada.");
        return false;
    }
    buffer[0].setTipo(lastType);
    symbolTable.atualizar(buffer[0]);
    stackAdd(Token("_", "L", lastType));
    printObjectFile(", ");
    printObjectFile(buffer[0].getLexema());
    return true;
}

bool Semantico::regra_8(){
    if (buffer[0].getTipo() != "NULO"){
        printErrorMessage("Variável já declarada.");
        return false;
    }
    buffer[0].setTipo(lastType);
    symbolTable.atualizar(buffer[0]);
    stackAdd(buffer[0]);
    printObjectFile(buffer[0].getLexema());
    return true;
}

bool Semantico::regra_9_10_11()
{
    Token token("_", "TIPO", buffer[0].getTipo());
    stackAdd(token);
    printObjectFile(convertTypes(token.getTipo())+" ");
    lastType = token.getTipo();
    return true;
}

bool Semantico::regra_12_24_32_39()
{
    stackAdd(Token("_", "A", "NULO"));
    return true;
}

bool Semantico::regra_13()
{
    if (buffer[1].getTipo() == "NULO"){
        printErrorMessage("Variável não declarada");
        return false;
    }
    stackAdd(Token("_", "ES", "NULO"));
    string auxType = "";
    if (buffer[1].getTipo() == "literal") auxType = "s";
    else if (buffer[1].getTipo() == "inteiro") auxType = "d";
    else if (buffer[1].getTipo() == "real") auxType = "lf";
    printObjectFile("scanf(\"%" + auxType + "\", " + (auxType == "s" ? "" : "&") + buffer[1].getLexema() + ");");
    return true;
}

bool Semantico::regra_14()
{
    stackAdd(Token("_", "ES", "NULO"));
    string auxType = "";
    if (buffer[1].getTipo() == "literal") auxType = "s";
    else if (buffer[1].getTipo() == "inteiro") auxType = "d";
    else if (buffer[1].getTipo() == "real") auxType = "lf";
    printObjectFile("printf(\"%" + auxType + "\", " + buffer[1].getLexema() + ");");
    return true;
}

bool Semantico::regra_15_16_21_23()
{
    Token token("_", buffer[0].getLexema(), buffer[0].getTipo());
    stackAdd(token);
    return true;
}

bool Semantico::regra_17()
{
    if (buffer[0].getTipo() == "NULO")
    {
        printErrorMessage("Variável não declarada");
        return false;
    }
    Token token("_", buffer[0].getLexema(), buffer[0].getTipo());
    stackAdd(token);
    return true;
}

bool Semantico::regra_18()
{
    stackAdd(Token("_", "A", "NULO"));
    return true;
}

bool Semantico::regra_19()
{
    if (buffer[0].getTipo() == "NULO")
    {
        printErrorMessage("Variável não declarada.");
        return false;
    }
    if (buffer[0].getTipo() != buffer[2].getTipo()){
        printErrorMessage("Tipos diferentes para atribuição.");
        return false;
    }
    stackAdd(Token("_", "CMD", "NULO"));
    printObjectFile(buffer[0].getLexema() + " = " + buffer[2].getLexema() + ";");
    return true;
}

bool Semantico::regra_20()
{
    if (buffer[0].getTipo() != buffer[2].getTipo() || buffer[0].getTipo() == "literal" || buffer[2].getTipo() == "literal"){
        printErrorMessage("Operandos com tipos incompatíveis.");
        return false;
    }
    string tempVar = "T_" + to_string(tempVars.size());
    tempVars.push_back({tempVar, buffer[0].getTipo()});
    stackAdd(Token("_", tempVar, buffer[0].getTipo()));
    printObjectFile(tempVar + " = " + buffer[0].getLexema() + " " + buffer[1].getLexema() + " " + buffer[2].getLexema() + ";");
    return true;
}

bool Semantico::regra_22()
{
    if (buffer[0].getTipo() == "NULO"){
        printErrorMessage("Variável não declarada.");
        return false;
    }
    stackAdd(Token("_", buffer[0].getLexema(), buffer[0].getTipo()));
    return true;
}

bool Semantico::regra_25()
{
    stackAdd(Token("_", "COND", "NULO"));
    printObjectFile("}");
    return true;
}

bool Semantico::regra_26()
{
    stackAdd(Token("_", "CAB", "NULO"));
    printObjectFile("if( " + buffer[2].getLexema() + " ){");
    return true;
}

bool Semantico::regra_27()
{
    if (buffer[0].getTipo() != buffer[2].getTipo() && (buffer[0].getTipo() == "literal" || buffer[2].getTipo() == "literal")){
        printErrorMessage("Operandos com tipos incompatíveis.");
        return false;
    }
    string tempVar = "T_" + to_string(tempVars.size());
    tempVars.push_back({tempVar, "inteiro"});
    stackAdd(Token("_", tempVar, "NULO"));
    lastExpr = buffer[0].getLexema() + " " + buffer[1].getLexema() + " " + buffer[2].getLexema();
    printObjectFile(tempVar + " = " + lastExpr + ";");
    return true;
}

bool Semantico::regra_28_29_30_31()
{
    stackAdd(Token("_", "CP", "NULO"));
    return true;
}

bool Semantico::regra_33()
{
    stackAdd(Token("_", "R", "NULO"));
    printObjectFile("}");
    return true;
}

bool Semantico::regra_34()
{
    stackAdd(Token("_", "CABR", "NULO"));
    eraseBytes(nbBytesPrintedLast);
    printObjectFile("while ( " + lastExpr + " ){");
    tempVars.pop_back();
    return true;
}

bool Semantico::regra_35_36_37_38()
{
    stackAdd(Token("_", "CP_R", "NULO"));
    return true;
}