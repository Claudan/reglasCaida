#ifndef Experto_H
#define Experto_H

#include "ArbolDinamico.h"
#include "Grafo.h"
#include "Regla.h"
#include "ReglaSemantica.h"

namespace ExpertoLib{

	class Experto
	{
    private:
        Nodo* raiz;
        NodoGrafo* raizGrafo;

    public:
        Experto();
        void insertaRegla(Regla nuevaRegla);
        void insertaReglaForward(Regla nuevaRegla);
        void insertaReglaSemantica(ReglaSemantica nuevaRegla);
        std::vector<std::string> infiereBackwardChaining(std::vector<std::string> input);
        void infiereBackwardChaining();
        void infiereForwardChaining();
        std::vector<std::vector<std::string>> infiereTodasLasSolucionesBC(std::vector<std::string> input);
        //void InfiereRedSemantica();
        void generaGrafico();
        void generaGrafico(std::string tipoEncadenamiento);
        void generaGraficoRedSemantica();
    };

}

#endif

