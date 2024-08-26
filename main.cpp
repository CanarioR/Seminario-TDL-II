#include <iostream>
#include "analizador.h"
#include "pila.h"

using namespace std;

// Tabla LR(1) ajustada
int tablaLR1[5][4] = {
    {2, 0, 0, 1}, // Estado 0
    {0, 0, -1, 0}, // Estado 1 (aceptación)
    {0, 3, 0, 0}, // Estado 2
    {4, 0, 0, 0}, // Estado 3
    {0, 0, -2, 0} // Estado 4 (reducción 1)
};

int main() {
    string entrada = "hola+mundo";
    Analizador analizador(entrada);
    Pila pila;
    int fila, columna, accion;
    bool aceptacion = false;

    pila.push(Tipo::FIN);
    pila.push(0);

    int tipo = analizador.sigSimbolo(); // Obtener el primer símbolo

    while (!aceptacion) {
        fila = pila.top(); // Estado actual en la cima de la pila
        columna = tipo;   // Tipo del símbolo actual
        accion = tablaLR1[fila][columna]; // Acción según la tabla

        // Mostrar pila y entrada
        pila.muestra();
        cout << "Entrada: " << tipo << endl;

        cout << "Accion: " << accion << endl << endl;
        switch (accion) {
            case -1: // Aceptación
                cout << "Cadena aceptada" << endl;
                aceptacion = true;
                break;

            case 0: // Error de análisis
                cout << "Error de analisis" << endl;
                aceptacion = true;
                break;

            case -2: // Reducción 1 (E -> id + id)
                pila.pop();
                pila.pop(); // Desaparecer $
                pila.pop(); // Desaparecer id
                pila.pop(); // Desaparecer +
                pila.pop(); // Desaparecer id
                pila.pop(); // Desaparecer estado (de id)
                pila.push(Tipo::E); // Empujar E en la pila
                pila.push(1);
                break;

            case 2: // Desplazamiento a estado 2
                pila.push(tipo); // Empujar el tipo actual (identificador)
                pila.push(accion); // Empujar el estado (2)
                tipo = analizador.sigSimbolo(); // Obtener el siguiente símbolo
                break;

            case 3: // Desplazamiento a estado 3
                pila.push(tipo); // Empujar el tipo actual (operador de adición)
                pila.push(accion); // Empujar el estado (3)
                tipo = analizador.sigSimbolo(); // Obtener el siguiente símbolo
                break;

            case 4: // Desplazamiento a estado 4
                pila.push(tipo); // Empujar el tipo actual (identificador)
                pila.push(accion); // Empujar el estado (4)
                tipo = analizador.sigSimbolo(); // Obtener el siguiente símbolo
                break;

            default:
                cout << "Acción desconocida: " << accion << endl;
                aceptacion = true;
                break;
        }
    }
    return 0;
}
