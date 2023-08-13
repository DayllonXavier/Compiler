#include <iostream>

#include "Token.hpp"
#include "Lexico.hpp"
#include "SymbolTable.hpp"
#include "Sintatico.hpp"
#include "Semantico.hpp"

using namespace std;

int main(int argc, char *argv[]){

    Token t;
    string path_to_source_code = argv[1];
    string path_to_object_code = argv[2];
    SymbolTable tabelaDeSimbolos;
    Lexico l(path_to_source_code, tabelaDeSimbolos);
    /*do
    {
        t = l.SCANNER();
        if (t.getClasse() != "ERRO")
            cout << t << endl;
    } while (t.getClasse() != "EOF");*/
    //tabelaDeSimbolos.imprimir();
    Semantico m(tabelaDeSimbolos, path_to_object_code);
    Sintatico s(l, m);
    bool f = s.process();
    cout << (f ? "TRUE" : "FALSE") << endl;
    return 0;
}
