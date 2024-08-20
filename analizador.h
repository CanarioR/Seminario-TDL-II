
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
    static const int FIN = 3;
    static const int OPADIC = 4;          // Operadores de adicion: +, -
    static const int OPMULT = 5;          // Operadores de multiplicacion: *, /
    static const int OPASIGNACION = 6;    // Operador de asignacion: =
    static const int OPREL = 7;           // Operadores relacionales: <, >, <=, >=, !=, ==
    static const int OPAND = 8;           // Operador AND: &&
    static const int OPOR = 9;            // Operador OR: ||
    static const int OPNOT = 10;          // Operador NOT: !
    static const int PAREN_IZQ = 11;      // Par�ntesis izquierdo: (
    static const int PAREN_DER = 12;      // Par�ntesis derecho: )
    static const int LLAVE_IZQ = 13;      // Llave izquierda: {
    static const int LLAVE_DER = 14;      // Llave derecha: }
    static const int PUNTO_Y_COMA = 15;   // Punto y coma: ;
    static const int IF = 16;             // Palabra reservada: if
    static const int WHILE = 17;          // Palabra reservada: while
    static const int RETURN = 18;         // Palabra reservada: return
    static const int ELSE = 19;           // Palabra reservada: else
    static const int INT = 20;            // Palabra reservada: int
    static const int FLOAT = 21;          // Palabra reservada: float

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
