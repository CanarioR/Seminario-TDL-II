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

string Analizador::tipoToString(int tipo) {
    switch (tipo) {
        case Tipo::IDENTIFICADOR: return "Identificador";
        case Tipo::ENTERO: return "Entero";
        case Tipo::REAL: return "Real";
        case Tipo::OPADIC: return "Operador Adicion";
        case Tipo::OPMULT: return "Operador Multiplicacion";
        case Tipo::OPASIGNACION: return "Operador Asignacion";
        case Tipo::OPREL: return "Operador Relacional";
        case Tipo::OPAND: return "Operador AND";
        case Tipo::OPOR: return "Operador OR";
        case Tipo::OPNOT: return "Operador NOT";
        case Tipo::PAREN_IZQ: return "Parentesis Izquierdo";
        case Tipo::PAREN_DER: return "Parentesis Derecho";
        case Tipo::LLAVE_IZQ: return "Llave Izquierda";
        case Tipo::LLAVE_DER: return "Llave Derecha";
        case Tipo::PUNTO_Y_COMA: return "Punto y Coma";
        case Tipo::IF: return "Palabra Reservada: if";
        case Tipo::WHILE: return "Palabra Reservada: while";
        case Tipo::RETURN: return "Palabra Reservada: return";
        case Tipo::ELSE: return "Palabra Reservada: else";
        case Tipo::INT: return "Palabra Reservada: int";
        case Tipo::FLOAT: return "Palabra Reservada: float";
        default: return "Error";
    }
}


void Analizador::entrada(string palabra){
    indice = 0;
    this->palabra = palabra;
}

int Analizador::sigSimbolo() {
    estado = 0;
    continuar = true;
    simbolo = "";

    while (continuar) {
        c = sigCaracter();

        if (c == '$') { // Fin de la cadena
            tipo = Tipo::FIN;
            continuar = false;
            break;
        }

        switch (estado) {
            case 0:
                if (esLetra(c)) sigEstado(1); // Identificadores o palabras reservadas
                else if (esDigito(c)) sigEstado(2); // Enteros o reales
                else if (c == '+' || c == '-'){valido(Tipo::OPADIC); tipo = Tipo::OPADIC;} // Operadores de adicion
                else if (c == '*' || c == '/') {valido(Tipo::OPMULT); tipo = Tipo::OPMULT;} // Operadores de multiplicacion
                else if (c == '=') sigEstado(5); // Operador de asignacion o relacional
                else if (c == '<' || c == '>') sigEstado(6); // Operadores relacionales
                else if (c == '!') sigEstado(7); // Operador NOT o relacional
                else if (c == '&') sigEstado(8); // Operador AND
                else if (c == '|') sigEstado(9); // Operador OR
                else if (c == '(') {valido(Tipo::PAREN_IZQ); tipo = Tipo::PAREN_IZQ;} // Parentesis izquierdo
                else if (c == ')') {valido(Tipo::PAREN_DER); tipo = Tipo::PAREN_DER;}// Parentesis derecho
                else if (c == '{') {valido(Tipo::LLAVE_IZQ);tipo = Tipo::LLAVE_IZQ;} // Llave izquierda
                else if (c == '}') {valido(Tipo::LLAVE_DER); tipo = Tipo::LLAVE_DER;} // Llave derecha
                else if (c == ';') {valido(Tipo::PUNTO_Y_COMA); tipo = Tipo::PUNTO_Y_COMA;} // Punto y coma
                else if (esEspacio(c)) continue; // Ignorar espacios
                else valido(Tipo::ERROR); // Cualquier otro simbolo 
                break;

            case 1:
                if (esLetra(c) || esDigito(c)) {
                    sigEstado(1); // Seguir leyendo identificadores
                } else {
                    retroceso();
                    if (esReservada(simbolo)) {
                        if (simbolo == "if") tipo = Tipo::IF;
                        else if (simbolo == "while") tipo = Tipo::WHILE;
                        else if (simbolo == "return") tipo = Tipo::RETURN;
                        else if (simbolo == "else") tipo = Tipo::ELSE;
                        else if (simbolo == "int") tipo = Tipo::INT;
                        else if (simbolo == "float") tipo = Tipo::FLOAT;
                    } else {
                        tipo = Tipo::IDENTIFICADOR;
                    }
                    continuar = false;
                }
                break;

            case 2:
                if (esDigito(c)) {
                    sigEstado(2);
                } else if (c == '.') {
                    sigEstado(15); // Detectar n�meros reales
                } else {
                    retroceso();
                    tipo = Tipo::ENTERO;
                    continuar = false;
                }
                break;

            case 5:
                if (c == '=') {
                    valido(Tipo::OPREL); // ==
                    tipo = Tipo::OPREL;
                } else {
                    retroceso();
                    tipo = Tipo::OPASIGNACION; // =
                    continuar = false;
                }
                break;

            case 6:
                if (c == '=') {
                    valido(Tipo::OPREL); // <= o >=
                    tipo = Tipo::OPREL;
                } else {
                    retroceso();
                    tipo = Tipo::OPREL; // < o >
                    continuar = false;
                }
                break;

            case 7:
                if (c == '=') {
                    valido(Tipo::OPREL); // !=
                    tipo = Tipo::OPREL;
                } else {
                    retroceso();
                    tipo = Tipo::OPNOT; // !
                    continuar = false;
                }
                break;

            case 8:
                if (c == '&') {
                    valido(Tipo::OPAND); // &&
                    tipo = Tipo::OPAND;
                } else {
                    retroceso();
                    tipo = Tipo::ERROR; // Error, un solo &
                    continuar = false;
                }
                break;

            case 9:
                if (c == '|') {
                    valido(Tipo::OPOR); // ||
                    tipo = Tipo::OPOR;
                } else {
                    retroceso();
                    tipo = Tipo::ERROR; // Error, un solo |
                    continuar = false;
                }
                break;

            case 15:
                if (esDigito(c)) {
                    sigEstado(15);
                } else {
                    retroceso();
                    tipo = Tipo::REAL;
                    continuar = false;
                }
                break;
        }
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

bool Analizador::esReservada(string palabra) {
    return palabra == "if" || palabra == "while" || palabra == "return" || palabra == "else" || palabra == "int" || palabra == "float";
}

