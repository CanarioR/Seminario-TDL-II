#include <iostream>
#include <cstdlib>
#include <string>

#include "analizador.h"

using namespace std;

int main() {
    string entrada = "if (1 > 3) return 5.0; else int x = a + b >= 2;";
    Analizador analizador(entrada);
    int tipo;

    do {
        tipo = analizador.sigSimbolo();
        cout << analizador.tipoToString(tipo) << " -> " << analizador.simbolo << endl;
    } while (tipo != Tipo::FIN);

    return 0;
}


