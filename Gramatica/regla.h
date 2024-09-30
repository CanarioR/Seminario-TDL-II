#ifndef REGLA_H
#define REGLA_H

#include <string>

struct Regla {
    int idNoTerminal;             // ID del no terminal en el lado izquierdo de la regla
    int longitud;                 // Cantidad de símbolos en el lado derecho de la regla
    std::string nombreNoTerminal; // Nombre del no terminal en el lado izquierdo de la regla
};

#endif // REGLA_H

