#include <iostream>
#include "analizador.h"
#include "pila.h"
#include "ElementoPila.h"

using namespace std;

// Tabla LR(1) ajustada
int tablaLR1[5][4] = {
    {2, 0, 0, 1}, // Estado 0
    {0, 0, -1, 0}, // Estado 1 (aceptaci�n)
    {0, 3, 0, 0}, // Estado 2
    {4, 0, 0, 0}, // Estado 3
    {0, 0, -2, 0} // Estado 4 (reducci�n 1)
};

int main() {
    string entrada = "a+b$";
    Analizador analizador(entrada);
    Pila pila;
    int fila, columna, accion;
    bool aceptacion = false;

    // Empujar el estado inicial
    pila.push(new Estado(0));

    // Obtener el primer s�mbolo de la entrada
    int tipo = analizador.sigSimbolo();

    while (!aceptacion) {
        // Obtener el estado en la cima de la pila
        ElementoPila* elemTop = pila.top();
        Estado* estadoTop = dynamic_cast<Estado*>(elemTop);
        if (!estadoTop) {
            cout << "Error: no hay estado en la cima de la pila" << endl;
            return -1;
        }

        fila = estadoTop->getValor();
        columna = tipo;   // Tipo del s�mbolo actual
        accion = tablaLR1[fila][columna]; // Acci�n seg�n la tabla

        // Mostrar pila y entrada
        pila.muestra();
        cout << "Entrada: " << entrada.substr(analizador.getIndice()-1) << endl;
        cout << "Acci�n: " << accion << endl << endl;

        switch (accion) {
            case -1: // Aceptaci�n
                cout << "Cadena aceptada" << endl;
                aceptacion = true;
                break;

            case 0: // Error de an�lisis
                cout << "Error de an�lisis" << endl;
                aceptacion = true;
                break;

            case -2: // Reducci�n 1 (E -> id + id)
                pila.pop(); // Estado despu�s de id
                pila.pop(); // id
                pila.pop(); // Estado despu�s de +
                pila.pop(); // +
                pila.pop(); // Estado despu�s de id
                pila.pop(); // id

                pila.push(new NoTerminal("E")); // Empujar NoTerminal "E"
                pila.push(new Estado(1)); // Estado despu�s de reducci�n
                break;

            case 2: // Desplazamiento a estado 2
                pila.push(new Terminal("id")); // Empujar el identificador (id)
                pila.push(new Estado(accion)); // Empujar el estado (2)
                tipo = analizador.sigSimbolo(); // Obtener el siguiente s�mbolo
                break;

            case 3: // Desplazamiento a estado 3
                pila.push(new Terminal("+")); // Empujar el operador de adici�n
                pila.push(new Estado(accion)); // Empujar el estado (3)
                tipo = analizador.sigSimbolo(); // Obtener el siguiente s�mbolo
                break;

            case 4: // Desplazamiento a estado 4
                pila.push(new Terminal("id")); // Empujar el identificador (id)
                pila.push(new Estado(accion)); // Empujar el estado (4)
                tipo = analizador.sigSimbolo(); // Obtener el siguiente s�mbolo
                break;

            default:
                cout << "Acci�n desconocida: " << accion << endl;
                aceptacion = true;
                break;
        }
    }

    return 0;
}
