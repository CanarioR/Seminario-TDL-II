#ifndef __ANALIZADOR
#define __ANALIZADOR

#include <iostream>
#include <string>

class Tipo
{
public:
    static const int ERROR = -1;
    static const int IDENTIFICADOR = 0;
    static const int ENTERO = 1;
    static const int REAL = 2;
    static const int CADENA = 3;
    static const int TIPO = 4;            // Palabra reservada: int, float
    static const int OPSUMA = 5;          // Operadores de suma: +, -
    static const int OPMUL = 6;           // Operadores de multiplicación: *, /
    static const int OPRELAC = 7;         // Operadores relacionales: <, >, <=, >=
    static const int OPOR = 8;            // Operador OR: ||
    static const int OPAND = 9;           // Operador AND: &&
    static const int OPNOT = 10;          // Operador NOT: !
    static const int OPIGUALDAD = 11;     // Operadores de igualdad: ==, !=
    static const int PUNTO_Y_COMA = 12;   // Punto y coma: ;
    static const int COMA = 13;           // Coma: ,
    static const int PAREN_IZQ = 14;      // Paréntesis izquierdo: (
    static const int PAREN_DER = 15;      // Paréntesis derecho: )
    static const int LLAVE_IZQ = 16;      // Llave izquierda: {
    static const int LLAVE_DER = 17;      // Llave derecha: }
    static const int ASIGNACION = 18;     // Operador de asignación: =
    static const int IF = 19;             // Palabra reservada: if
    static const int WHILE = 20;          // Palabra reservada: while
    static const int RETURN = 21;         // Palabra reservada: return
    static const int ELSE = 22;           // Palabra reservada: else
    static const int FIN = 23;            // Fin de cadena: $
};

class Analizador
{
public:
    std::string simbolo;
    int tipo;

    Analizador(std::string palabra);
    Analizador();

    void entrada(std::string palabra);
    std::string tipoToString(int tipo);

    int sigSimbolo();
    bool terminado();

private:
    std::string palabra;

    int indice, estado;
    bool continuar;
    char c;

    char sigCaracter();
    void sigEstado(int);
    void valido(int);
    bool esLetra(char);
    bool esDigito(char);
    bool esEspacio(char);
    void retroceso();
    bool esReservada(std::string);
};

#endif
