#include "analizador.h"

// Constructor con parámetro
Analizador::Analizador(std::string palabra) : palabra(palabra), indice(0), estado(0), continuar(true) {}

// Constructor vacío
Analizador::Analizador() : indice(0), estado(0), continuar(true) {}

// Función para recibir la entrada
void Analizador::entrada(std::string palabra) {
    this->palabra = palabra;
    indice = 0;
    estado = 0;
    continuar = true;
}

// Función que regresa el siguiente símbolo y su tipo
int Analizador::sigSimbolo() {
    while (continuar) {
        c = sigCaracter();

        if (esEspacio(c)) {
            continue;
        } else if (esLetra(c)) {
            simbolo = c;
            while (esLetra(c = sigCaracter()) || esDigito(c)) {
                simbolo += c;
            }
            retroceso();
            if (esReservada(simbolo)) {
                return tipo; // Palabra reservada
            } else {
                tipo = Tipo::IDENTIFICADOR;
                return Tipo::IDENTIFICADOR;
            }
        } else if (esDigito(c)) {
            simbolo = c;
            while (esDigito(c = sigCaracter())) {
                simbolo += c;
            }
            if (c == '.') {
                simbolo += c;
                while (esDigito(c = sigCaracter())) {
                    simbolo += c;
                }
                retroceso();
                tipo = Tipo::REAL;
                return Tipo::REAL;
            } else {
                retroceso();
                tipo = Tipo::ENTERO;
                return Tipo::ENTERO;
            }
        } else {
            // Otros casos: operadores, signos de puntuación, etc.
            switch (c) {
                case '+':
                case '-':
                    tipo = Tipo::OPSUMA;
                    simbolo = c;
                    return Tipo::OPSUMA;
                case '*':
                case '/':
                    tipo = Tipo::OPMUL;
                    simbolo = c;
                    return Tipo::OPMUL;
                case '<':
                case '>': {
                    simbolo = c;
                    if ((c = sigCaracter()) == '=') {
                        simbolo += c;
                        tipo = Tipo::OPRELAC;
                        return Tipo::OPRELAC;
                    } else {
                        retroceso();
                        tipo = Tipo::OPRELAC;
                        return Tipo::OPRELAC;
                    }
                }
                case '!': {
                    simbolo = c;
                    c = sigCaracter();
                    if (c == '=') {
                        simbolo += c;
                        tipo = Tipo::OPIGUALDAD;
                        return Tipo::OPIGUALDAD;
                    } else {
                        retroceso();
                        tipo = Tipo::OPNOT;
                        return Tipo::OPNOT;
                    }
                }
                case '=': {
                    simbolo = c;
                    c = sigCaracter();
                    if (c == '=') {
                        simbolo += c;
                        tipo = Tipo::OPIGUALDAD;
                        return Tipo::OPIGUALDAD;
                    } else {
                        retroceso();
                        tipo = Tipo::ASIGNACION;
                        return Tipo::ASIGNACION;
                    }
                }
                case '|': {
                    simbolo = c;
                    c = sigCaracter();
                    if (c == '|') {
                        simbolo += c;
                        tipo = Tipo::OPOR;
                        return Tipo::OPOR;
                    } else {
                        tipo = Tipo::ERROR;
                        return Tipo::ERROR;
                    }
                }
                case '&': {
                    simbolo = c;
                    c = sigCaracter();
                    if (c == '&') {
                        simbolo += c;
                        tipo = Tipo::OPAND;
                        return Tipo::OPAND;
                    } else {
                        tipo = Tipo::ERROR;
                        return Tipo::ERROR;
                    }
                }
                case ';':
                    tipo = Tipo::PUNTO_Y_COMA;
                    simbolo = c;
                    return Tipo::PUNTO_Y_COMA;
                case ',':
                    tipo = Tipo::COMA;
                    simbolo = c;
                    return Tipo::COMA;
                case '(':
                    tipo = Tipo::PAREN_IZQ;
                    simbolo = c;
                    return Tipo::PAREN_IZQ;
                case ')':
                    tipo = Tipo::PAREN_DER;
                    simbolo = c;
                    return Tipo::PAREN_DER;
                case '{':
                    tipo = Tipo::LLAVE_IZQ;
                    simbolo = c;
                    return Tipo::LLAVE_IZQ;
                case '}':
                    tipo = Tipo::LLAVE_DER;
                    simbolo = c;
                    return Tipo::LLAVE_DER;
                case '$':
                    tipo = Tipo::FIN;
                    simbolo = c;
                    continuar = false;
                    return Tipo::FIN;
                default:
                    tipo = Tipo::ERROR;
                    simbolo = c;
                    return Tipo::ERROR;
            }
        }
    }
}
    // Función que convierte el tipo a string (para fines de depuración o presentación)
std::string Analizador::tipoToString(int tipo) {
        switch (tipo) {
            case Tipo::IDENTIFICADOR: return "Identificador";
            case Tipo::ENTERO: return "Entero";
            case Tipo::REAL: return "Real";
            case Tipo::CADENA: return "Cadena";
            case Tipo::TIPO: return "Tipo";
            case Tipo::OPSUMA: return "Operador_Suma";
            case Tipo::OPMUL: return "Operador_Multiplicación";
            case Tipo::OPRELAC: return "Operador_Relacional";
            case Tipo::OPOR: return "Operador_OR";
            case Tipo::OPAND: return "Operador_AND";
            case Tipo::OPNOT: return "Operador_NOT";
            case Tipo::OPIGUALDAD: return "Operador_Igualdad";
            case Tipo::PUNTO_Y_COMA: return "Punto_y_Coma";
            case Tipo::COMA: return "Coma";
            case Tipo::PAREN_IZQ: return "Parentesis_Izquierdo";
            case Tipo::PAREN_DER: return "Parentesis_Derecho";
            case Tipo::LLAVE_IZQ: return "Llave_Izquierda";
            case Tipo::LLAVE_DER: return "Llave_Derecha";
            case Tipo::ASIGNACION: return "Asignacion";
            case Tipo::IF: return "If";
            case Tipo::WHILE: return "While";
            case Tipo::RETURN: return "Return";
            case Tipo::ELSE: return "Else";
            case Tipo::FIN: return "Fin_de_Cadena";
            default: return "Error";
        }
    }

    // Función que verifica si la cadena es una palabra reservada
    bool Analizador::esReservada(std::string palabra) {
        if (palabra == "if") {
            tipo = Tipo::IF;
        } else if (palabra == "while") {
            tipo = Tipo::WHILE;
        } else if (palabra == "return") {
            tipo = Tipo::RETURN;
        } else if (palabra == "else") {
            tipo = Tipo::ELSE;
        } else if (palabra == "int" || palabra == "float") {
            tipo = Tipo::TIPO;
        } else {
            return false;
        }
        return true;
    }

    // Funciones auxiliares

    char Analizador::sigCaracter() {
        return (indice < palabra.size()) ? palabra[indice++] : '$';
    }

    void Analizador::retroceso() {
        if (indice > 0) --indice;
    }

    bool Analizador::esLetra(char c) {
        return std::isalpha(static_cast<unsigned char>(c));
    }

    bool Analizador::esDigito(char c) {
        return std::isdigit(static_cast<unsigned char>(c));
    }

    bool Analizador::esEspacio(char c) {
        return std::isspace(static_cast<unsigned char>(c));
    }

    bool Analizador::terminado() {
        return !continuar;
    }
