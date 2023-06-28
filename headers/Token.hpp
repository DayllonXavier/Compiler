#ifndef Token_hpp
#define Token_hpp

#include <iostream>
#include <string>

using namespace std;

class Token
{
    public:
        Token();
        ~Token();
        Token(string valor);
        Token(string _classe, string _lexema, string _tipo);
        string getClasse() const;
        string getLexema() const;
        string getTipo() const;
        void setClasse(string valor);
        void setLexema(string valor);
        void setTipo(string valor);

        friend ostream& operator<<(ostream& os, const Token & token)
        {
            return os << "Classe: " << token.getClasse() << ", Lexema: " << token.getLexema() << ", Tipo: " << token.getTipo();    
        }

        bool operator<(const Token& token) const;
        bool operator>(const Token& token) const;
        bool operator==(const Token& token) const;
        bool operator!=(const Token& token) const;

    private:
        string classe;
        string lexema;
        string tipo;
};

#endif //Token_hpp