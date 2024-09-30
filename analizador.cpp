#include "analizador.h"

using namespace std;

Analizador::Analizador(string palabra) : palabra(palabra), indice(0), estado(0), continuar(true) {
    tipo = Tipo::ERROR; // Inicializar el tipo en error
}

Analizador::Analizador() : indice(0), estado(0), continuar(true) {
    tipo = Tipo::ERROR; // Inicializar el tipo en error
}

string Analizador::tipoToString(int tipo) {
    switch (tipo) {
        case Tipo::IDENTIFICADOR: return "Identificador";
        case Tipo::OPADICION: return "Operador Adición";
        case Tipo::FIN: return "Fin";
        case Tipo::E: return "E";
        default: return "Error";
    }
}

void Analizador::entrada(string palabra) {
    this->palabra = palabra;
    indice = 0;
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
            else if (c == '+')
            {
                return Tipo::OPADICION;
            }
            else if (c == '$')
            {
                return Tipo::FIN;
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
        tipo = Tipo::OPADICION;
        break;
    case 3:
        tipo = Tipo::FIN;
        break;
    default:
        tipo = Tipo::ERROR;
    }

    return tipo;
}

char Analizador::sigCaracter() {
    if (terminado()) return '$';
    return palabra[indice++];
}

void Analizador::sigEstado(int estado) {
    this->estado = estado;
    simbolo += c;
}

void Analizador::valido(int estado) {
    sigEstado(estado);
    continuar = false;
}

bool Analizador::terminado() {
    return indice >= palabra.length();
}

bool Analizador::esLetra(char c) {
    return isalpha(c) || c == '_';
}

bool Analizador::esDigito(char c) {
    return isdigit(c);
}

bool Analizador::esEspacio(char c) {
    return c == ' ' || c == '\t';
}

void Analizador::retroceso() {
    if (c != '$') indice--;
    continuar = false;
}
