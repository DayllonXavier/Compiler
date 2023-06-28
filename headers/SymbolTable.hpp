#ifndef SymbolTable_hpp
#define SymbolTable_hpp

#include <set>
#include "Token.hpp"

using namespace std;



class SymbolTable
{
    public:
        SymbolTable();
        ~SymbolTable();
        void imprimir();
        void inserir(Token token);
        void atualizar(Token token);
        Token pesquisar(string lexema);
        void excluir(string lexema);

        set<Token> tabelaDeSimbolos;
};


#endif
