#ifndef ReglaSemantica_H
#define ReglaSemantica_H

#include <string>
#include <iostream>

namespace ExpertoLib {

    class ReglaSemantica
    {
    private:
        std::string valor;
        std::string arco;
        std::string hijo;

    public:
        ReglaSemantica(std::string valor, std::string arco, std::string hijo);
        std::string getValorPadre();
        std::string getValorArco();
        std::string getValorHijo();
    };

}

#endif