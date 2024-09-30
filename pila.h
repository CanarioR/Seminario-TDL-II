#ifndef __PILA_H__
#define __PILA_H__

#include <stack>
#include "ElementoPila.h"

class Pila {
public:
    void push(ElementoPila* elem); // Empuja un objeto de tipo ElementoPila en la pila
    void pop();
    ElementoPila* top();
    bool empty();
    void muestra();

private:
    std::stack<ElementoPila*> s; // Cambiamos a una pila de punteros a ElementoPila
};

#endif
