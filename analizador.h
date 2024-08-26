#ifndef __ANALIZADOR
#define __ANALIZADOR

#include <iostream>
#include <string>

class Tipo
{
public:
    static const int ERROR = -1;
    static const int IDENTIFICADOR = 0;
    static const int OPADICION = 1;
    static const int FIN = 2;
    static const int E = 3;
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
};

#endif
