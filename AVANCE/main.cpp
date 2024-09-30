// main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "analizador.h"
#include "pila.h"

using namespace std;

// Estructura para una regla de producción
struct ReglaProduccion {
    int id; // ID del no terminal
    int numSimbolos; // Cantidad de símbolos en el lado derecho
    string nombre; // Nombre del no terminal
};

struct TablaLR {
    vector<vector<int>> tabla; // Tabla LR(1)
    vector<string> simbolos; // Lista de símbolos
    vector<ReglaProduccion> producciones; // Lista de reglas de producción
    int numFilas;
    int numColumnas;
};

bool cargarTablaLR(TablaLR& tabla, const string& nombreArchivo);
void analizarCadena(Analizador& analizador, TablaLR& tabla);
void printTablaLR(const TablaLR& tabla);

int main() {
    // Cargar la tabla LR
    TablaLR tabla;
    if (!cargarTablaLR(tabla, "tabla.lr")) {
        cerr << "Error al cargar la tabla LR." << endl;
        return 1;
    }

    //printTablaLR(tabla);

    string entrada;
    cout << "Ingrese la cadena a analizar: ";
    getline(cin, entrada);

    // Inicializar el analizador léxico
    Analizador analizador(entrada);
    analizarCadena(analizador, tabla);

    return 0;
}

bool cargarTablaLR(TablaLR& tabla, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return false;
    }

    // Leer el número de reglas
    int numReglas;
    archivo >> numReglas;

    // Leer cada regla y almacenarla
    tabla.producciones.reserve(numReglas);
    for (int i = 0; i < numReglas; ++i) {
        ReglaProduccion regla;
        archivo >> regla.id >> regla.numSimbolos;
        archivo.ignore(); // Ignorar el tabulador o espacio antes del nombre
        getline(archivo, regla.nombre); // Leer el nombre del no terminal

        tabla.producciones.push_back(regla);
    }

    // Leer las dimensiones de la tabla LR(1)
    archivo >> tabla.numFilas >> tabla.numColumnas;

    // Leer la tabla LR(1)
    tabla.tabla.resize(tabla.numFilas, vector<int>(tabla.numColumnas, 0));
    for (int i = 0; i < tabla.numFilas; ++i) {
        for (int j = 0; j < tabla.numColumnas; ++j) {
            if (!(archivo >> tabla.tabla[i][j])) {
                cerr << "Error al leer la tabla LR en la posición (" << i << ", " << j << ")." << endl;
                archivo.close();
                return false;
            }
        }
    }

    archivo.close();

    // símbolos predefinidos
    tabla.simbolos = {
        "Identificador", "Entero", "Real", "Cadena", "Tipo",
        "Operador_Suma", "Operador_Multiplicacion", "Operador_Relacional",
        "Operador_OR", "Operador_AND", "Operador_NOT", "Operador_Igualdad",
        "Punto_y_Coma", "Coma", "Parentesis_Izquierdo", "Parentesis_Derecho",
        "Llave_Izquierda", "Llave_Derecha", "Asignacion", "If",
        "While", "Return", "Else", "Fin_de_Cadena",
        "programa", "Definiciones", "Definicion", "DefVar", "ListaVar",
        "DefFunc", "Parametros", "ListaParam", "BloqFunc", "DefLocales",
        "DefLocal", "Sentencias", "Sentencia", "Otro", "Bloque",
        "ValorRegresa", "Argumentos", "ListaArgumentos", "Termino",
        "LlamadaFunc", "SentenciaBloque", "Expresion"
    };

    // Verificar el no terminal
    for (const auto& regla : tabla.producciones) {
        bool encontrado = false;
        for (const auto& simbolo : tabla.simbolos) {
            if (simbolo == regla.nombre) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            cerr << "Advertencia: No terminal " << regla.nombre << " no está en la lista de símbolos. Agregándolo automáticamente." << endl;
            tabla.simbolos.push_back(regla.nombre);
        }
    }

    // Verificar el número de símbolos
    if (tabla.simbolos.size() != static_cast<size_t>(tabla.numColumnas)) {
        cerr << "Advertencia: El número de símbolos (" << tabla.simbolos.size()
                  << ") no coincide con el número de columnas de la tabla LR(1) ("
                  << tabla.numColumnas << ")." << endl;
    }

    return true;
}
void analizarCadena(Analizador& analizador, TablaLR& tabla) {
    Pila pila;
    pila.push(new Estado(0));  // Estado inicial

    int token = analizador.sigSimbolo();  // Obtener primer token

    while (true) {
        Estado* estadoActual = dynamic_cast<Estado*>(pila.top());
        if (!estadoActual) {
            cerr << "Error: Estado no encontrado en la pila." << endl;
            break;
        }
        int estado = estadoActual->getValor();

        cout << "\nEstado actual: " << estado << ", Token: " << analizador.tipoToString(token) << endl;

        // Buscar la acción en la tabla
        int columna = -1;
        for (size_t i = 0; i < tabla.simbolos.size(); ++i) {
            if (tabla.simbolos[i] == analizador.tipoToString(token)) {
                columna = i;
                break;
            }
        }

        if (columna == -1) {
            cerr << "Error: simbolo no reconocido: " << analizador.tipoToString(token) << endl;
            break;
        }

        cout << "Token '" << analizador.tipoToString(token) << "' mapeado a columna: " << columna << endl;

        int accion = tabla.tabla[estado][columna];

        if (accion > 0) {  // Acción de desplazamiento
            cout << "Desplazamiento al estado: " << accion << endl;
            pila.push(new Terminal(analizador.simbolo));
            pila.push(new Estado(accion));
            token = analizador.sigSimbolo();  // Obtener siguiente token
        } else if (accion < 0) {  // Acción de reducción
            int reglaIndex = -accion -2; // Índice en el vector
            if (reglaIndex < 0 || reglaIndex >= tabla.producciones.size()) {
                cerr << "Error: regla de reducción inválida: " << accion << endl;
                break;
            }

            ReglaProduccion regla = tabla.producciones[reglaIndex];
            cout << "Reducción usando la regla: " << regla.nombre << " -> " << regla.numSimbolos << " símbolos" << endl;

            // Desapilar 2 * numSimbolos elementos de la pila (símbolos y estados)
            for (int i = 0; i < regla.numSimbolos * 2; ++i) {
                pila.pop();
                if (pila.empty()) {
                    cerr << "Error: pila vacía al intentar desapilar." << endl;
                    break;
                }
            }

            // Obtener el estado actual después de desapilar
            Estado* nuevoEstado = dynamic_cast<Estado*>(pila.top());
            if (!nuevoEstado) {
                cerr << "Error: Estado no encontrado después de desapilar." << endl;
                break;
            }

            int nuevoEstadoVal = nuevoEstado->getValor();

            // Obtener el símbolo no terminal de la regla
            string noTerminal = regla.nombre;

            // Buscar la columna correspondiente al no terminal
            int columnaNT = -1;
            for (size_t i = 0; i < tabla.simbolos.size(); ++i) {
                if (tabla.simbolos[i] == noTerminal) {
                    columnaNT = i;
                    break;
                }
            }

            if (columnaNT == -1) {
                cerr << "Error: símbolo no terminal no encontrado en la tabla: " << noTerminal << endl;
                break;
            }

            // Obtener la acción de ir a
            int gotoAccion = tabla.tabla[nuevoEstadoVal][columnaNT];
            if (gotoAccion <= 0) {
                cerr << "Error: acción de goto inválida para no terminal " << noTerminal << " en estado " << nuevoEstadoVal << endl;
                break;
            }

            cout << "Ir a estado: " << gotoAccion << " con no terminal: " << noTerminal << endl;
            pila.push(new NoTerminal(noTerminal));
            pila.push(new Estado(gotoAccion));

        } else { // accion == 0
            cerr << "Error: acción no válida (0)." << endl;
            break;
        }

        // Imprimir el estado de la pila
        pila.muestra();
    }
}
