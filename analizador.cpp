#include "analizador.h"

using namespace std;

Analizador::Analizador(string palabra)
{
    indice = 0;
    this->palabra = palabra;
}

Analizador::Analizador()
{
    indice = 0;
}

string Analizador::tipoToString(int tipo)
{
    string str = "";
    switch (tipo)
    {
    case Tipo::IDENTIFICADOR:
        str = "Identificador";
        break;
    case Tipo::ENTERO:
        str = "Entero";
        break;
    case Tipo::REAL:
        str = "Real";
        break;
    default:

        break;
    }

    return str;
}

int Analizador::sigSimbolo()
{

    estado = 0;
    continuar = true;
    simbolo = "";

    while (continuar)
    {
        c = sigCaracter();

        switch (estado)
        {
        case 0:
            if (esLetra(c))
            {
                sigEstado(1);
            }
            else if (esDigito(c))
            {
                sigEstado(2);
            }
            else if (esEspacio(c))
            {
                ///
            }
            else if (c == '$')
            {
                valido(4); // FIN
            }
            else
            {
                valido(-1);
            }
            break;
        case 1:
            if (esLetra(c) || esDigito(c))
            {
                sigEstado(1); // Parte del identificador
            }
            else
            {
                valido(1);   // Identificador completo
                retroceso(); // Retrocede para no consumir el siguiente símbolo
            }
            break;

        case 2:
            if (esDigito(c))
            {
                sigEstado(2); // Entero
            }
            else if (c == '.')
            {
                sigEstado(3); // Puede ser un número real
            }
            else
            {
                valido(2); // Entero completo
                retroceso();
            }
            break;

        case 3:
            if (esDigito(c))
            {
                sigEstado(3); // Parte fraccionaria de un número real
            }
            else
            {
                valido(3); // Real completo
                retroceso();
            }
            break;

        default:
            valido(-1); // Error
            break;
        }
    }

    switch (estado) {
    case 1:
        tipo = Tipo::IDENTIFICADOR;
        break;
    case 2:
        tipo = Tipo::ENTERO;
        break;
    case 3:
        tipo = Tipo::REAL;
        break;
    case 4:
        tipo = Tipo::FIN;
        break;
    default:
        tipo = Tipo::ERROR;
    }

    return tipo;
}

char Analizador::sigCaracter(){
    if(terminado()) return '$';
    return palabra[indice++];
}

void Analizador::sigEstado(int estado){
    this->estado = estado;
    simbolo += c;
}

void Analizador::valido(int estado){
    sigEstado(estado);
    continuar = false;
}

bool Analizador::terminado(){
    return indice >= palabra.length();
}

bool Analizador::esLetra(char c){
    return isalpha(c) || c == '_';
}

bool Analizador::esDigito(char c){
    return isdigit(c);
}

bool Analizador::esEspacio(char c){
    return c == ' ' || c == '\t';
}

void Analizador::retroceso(){
    if(c != '$') indice--;
    continuar = false;
}