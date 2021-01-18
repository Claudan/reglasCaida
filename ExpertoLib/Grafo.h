#ifndef Grafo_H
#define Grafo_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>  

#include <queue>

namespace ExpertoLib {

    struct NodoGrafo
    {
        std::string valor;
        std::vector<std::string> arco;
        std::vector<NodoGrafo*> hijo;
    };

    NodoGrafo* nuevoNodoGrafo(std::string valor)
    {
        NodoGrafo* temp = new NodoGrafo;
        temp->valor = valor;
        return temp;
    }

    NodoGrafo* buscaNodoGrafo(std::string valor, NodoGrafo* nodo)
    {
        if (valor == nodo->valor) {
            return nodo;
        }
        for (unsigned int i = 0; i < (nodo->hijo).size(); i++) {
            NodoGrafo* temp = buscaNodoGrafo(valor, (nodo->hijo[i]));
            if (temp != NULL) {
                return temp;
            }
        }
        return NULL;
    }

    void insertaANodoGrafo(NodoGrafo* Padre, NodoGrafo* Hijo, NodoGrafo* raizGrafo, std::string arco) {
        bool nodoPosicionado = false;
        for (unsigned int i = 0; i < (raizGrafo->hijo).size(); i++) {
            if (nodoPosicionado == false) {
                if (raizGrafo->hijo[i]->valor == Padre->valor) {
                    (raizGrafo->hijo[i]->arco).push_back(arco);
                    (raizGrafo->hijo[i]->hijo).push_back(Hijo);
                    nodoPosicionado = true;
                }
                else {
                    insertaANodoGrafo(Padre,Hijo,raizGrafo->hijo[i],arco);
                }
            }
        }
    }

    void recorreGrafo(NodoGrafo* p, std::ofstream& outfile)
    {
        char comilla = '"';
        std::string padre = p->valor;
        for (unsigned int i = 0; i < p->hijo.size(); i++)
        {
            if (padre != "Raiz") {
                outfile << comilla << padre << comilla << "->";
                outfile << comilla << p->hijo[i]->valor << comilla << std::endl;
                if ((p->arco).size() > 0) {
                    outfile << "[label = " << comilla << p->arco[i] << comilla << "];" << std::endl;
                }
            }
            recorreGrafo(p->hijo[i], outfile);
        }
    }

    void generaGraficoGrafo(NodoGrafo* raiz)
    {
        if (!raiz)
            return;
        char comilla = '"';
        std::ofstream outfile("red_semantica.dot");
        outfile << "strict digraph G {" << std::endl;
        outfile << "rankdir = LR;" << std::endl;
        outfile << "graph [ordering=" << comilla << "out" << comilla << "];" << std::endl;
        outfile << "graph [ranksep=" << comilla << "1.5" << comilla << "];" << std::endl;
        recorreGrafo(raiz, outfile);
        outfile << "}" << std::endl;
        outfile.close();
    }

}

#endif
