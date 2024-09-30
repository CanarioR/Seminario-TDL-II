#include "pila.h"

void Pila::push(ElementoPila* elem) {
    s.push(elem);
}

void Pila::pop() {
    if (!s.empty()) {
        ElementoPila* elem = s.top();
        delete elem; // Liberar la memoria
        s.pop();
    }
}

ElementoPila* Pila::top() {
    if (!s.empty()) return s.top();
    return nullptr;
}

bool Pila::empty() {
    return s.empty();
}

void Pila::muestra() {
    std::stack<ElementoPila*> temp = s;
    std::cout << "Pila: ";
    while (!temp.empty()) {
        std::cout << temp.top()->toString() << " ";
        temp.pop();
    }
    std::cout << std::endl;
}
