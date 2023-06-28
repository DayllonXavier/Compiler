#include "SymbolTable.hpp"

SymbolTable::SymbolTable()
{
    tabelaDeSimbolos.clear();
}
SymbolTable::~SymbolTable()
{
    tabelaDeSimbolos.clear();
}
void SymbolTable::inserir(Token token)
{
    tabelaDeSimbolos.insert(token);
}

void SymbolTable::atualizar(Token token)
{
    excluir(token.getLexema());
    inserir(token);
}

Token SymbolTable::pesquisar(string lexema)
{
    auto tokenPtr = tabelaDeSimbolos.find(Token(lexema));
    if (tokenPtr == tabelaDeSimbolos.end())
        return Token();
    return *tokenPtr;
}

void SymbolTable::excluir(string lexema)
{
    tabelaDeSimbolos.erase(Token(lexema));
}

void SymbolTable::imprimir()
{
    cout << "TABELA DE SIMBOLOS\n";
    for (Token token : tabelaDeSimbolos){
        cout << token << "\n";
    }
}
