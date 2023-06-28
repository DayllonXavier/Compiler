#ifndef Lexico_hpp
#define Lexico_hpp

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <assert.h>
#include <vector>

#include "Token.hpp"
#include "SymbolTable.hpp"

using namespace std;

class Lexico
{
    public:
        //Lexico();
        Lexico(string _pathFileProgramaFonte, SymbolTable& _tabelaDeSimbolos);
        ~Lexico();
        Token SCANNER();
        pair<int, int> getPos();
    private:
        void carregarPalavrasReservadas();
        void abrirProgramaFonte();
        void readNextCaracter();
        void carregarAutomato();
        void desfazerLeitura();

        string pathFilePalavrasReservadas = "config/lexico/palavrasReservadas.txt";
        string pathFileAutomato = "config/lexico/AutomatoLexico.txt";
        string pathFileProgramaFonte;

        SymbolTable& tabelaDeSimbolos;
        map<char, int> idxOfCaracter;
        vector<vector<int>> transicoesAutomato;
        map<int, string> estadosFinais, mensagensErro, tiposDosEstados;
        int linha, prev_coluna, coluna, estado;
        string buffer;
        char caracter;
        ifstream fileProgramaFonte;
};

#endif //Lexico_hpp
