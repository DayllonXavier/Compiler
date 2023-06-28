#include "Token.hpp"

Token::Token()
{
}

Token::~Token()
{
}

Token::Token(string valor)
{
    setClasse(valor);
    setLexema(valor);
    setTipo(valor);
}

Token::Token(string _classe, string _lexema, string _tipo)
{
    setClasse(_classe);
    setLexema(_lexema);
    setTipo(_tipo);
}

string Token::getClasse() const
{
    return classe;
}

string Token::getLexema() const
{
    return lexema;
}

string Token::getTipo() const 
{
    return tipo;
}

void Token::setClasse(string valor)
{
    classe = valor;
}

void Token::setLexema(string valor)
{
    lexema = valor;
}

void Token::setTipo(string valor)
{
    tipo = valor;
}

bool Token::operator<(const Token& token) const
{
    return getLexema() < token.getLexema();
}

bool Token::operator>(const Token& token) const
{
    return getLexema() > token.getLexema();
}

bool Token::operator==(const Token& token) const
{
    return getLexema() == token.getLexema();
}

bool Token::operator!=(const Token& token) const
{
    return getLexema() != token.getLexema();
}