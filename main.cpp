#include <iostream>
#include <cstdlib>
#include <string>

#include "analizador.h"

using namespace std;

int main(){
    string entrada = "var123 21 21.21 .2";
    Analizador analizador(entrada);

    while(analizador.sigSimbolo() != Tipo::FIN){
        cout << analizador.tipoToString(analizador.sigSimbolo()) << endl;
    }
}