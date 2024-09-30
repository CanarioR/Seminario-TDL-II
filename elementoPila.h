#ifndef __ELEMENTO_PILA_H__
#define __ELEMENTO_PILA_H__

#include <iostream>
#include <string>

class ElementoPila {
public:
    virtual std::string toString() = 0; // Método abstracto para representar el elemento como una cadena
    virtual ~ElementoPila() = default;  // Destructor virtual
};

// Clase Terminal, que representa un símbolo terminal (como identificadores, operadores, etc.)
class Terminal : public ElementoPila {
private:
    std::string simbolo; // Puede ser el valor del terminal (como "id", "+", "$", etc.)
public:
    Terminal(std::string simbolo) : simbolo(simbolo) {}
    std::string toString() override { return simbolo; } // Devuelve el símbolo como cadena
};

// Clase NoTerminal, que representa un símbolo no terminal (como "E")
class NoTerminal : public ElementoPila {
private:
    std::string simbolo; // El no terminal (como "E")
public:
    NoTerminal(std::string simbolo) : simbolo(simbolo) {}
    std::string toString() override { return simbolo; } // Devuelve el símbolo como cadena
};

// Clase Estado, que representa un número de estado en el autómata LR(1)
class Estado : public ElementoPila {
public:
    int valor; // Valor del estado

    Estado(int v) : valor(v) {}
    int getValor() { return valor; }

    std::string toString() override {
        return "Estado(" + std::to_string(valor) + ")";
    }
};


#endif
