#ifndef ArbolDinamico_H
#define ArbolDinamico_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>  

#include <queue>

namespace ExpertoLib {
    
    struct Nodo
    {
        std::string valor;
        std::string comentario;
        bool esHoja = false;
        bool fueAceptado = false;
        bool fueRechazado = false;
        std::vector<Nodo*> hijo;
    };

    Nodo* nuevoNodo(std::string valor, std::string comentario, bool esHoja)
    {
        Nodo* temp = new Nodo;
        temp->valor = valor;
        temp->comentario = comentario;
        temp->esHoja = esHoja;
        return temp;
    }

    Nodo* buscaNodo(std::string valor, Nodo* nodo)
    {
        if (valor == nodo->valor)
            return nodo;
        for (unsigned int i = 0; i < (nodo->hijo).size(); i++) {
            Nodo* temp = buscaNodo(valor, (nodo->hijo[i]));
            if (temp != NULL) {
                return temp;
            }
        }
        return NULL;
    }

    // Algoritmo DFS para backward chaining
    bool esEstaConsecuencia(Nodo* nodo)
    {
        int respuesta = 0;
        unsigned int i = 0;
        //comprueba rechazo previo
        for (i = 0; i < (nodo->hijo).size(); i++)
        {
            if (nodo->hijo[i]->fueRechazado) {
                return false;
            }
        }
        for (i = 0; i < (nodo->hijo).size(); i++) 
        {
            if (!esEstaConsecuencia(nodo->hijo[i])){
                nodo->fueRechazado = true;
                return false;
            }
        }
        if (nodo->esHoja) //o que es lo mismo, el nodo es hoja i == 0 
        {
            if (nodo->fueAceptado)
                return true;
            if (nodo->fueRechazado)
                return false;
            while ( respuesta != 1 && respuesta !=2 ) {
                std::cout << nodo->valor << " ? (1:si/2:no)\n";
                std::cin >> respuesta;
            }
            if (respuesta == 1){
                nodo->fueAceptado = true;
                return true;
            }
            else{
                nodo->fueRechazado = true;
                return false;
            }
        }
        //pasamos todos los filtros, es decir, tiene hijos y no es un nodo hoja
        return true;
    }
    
    bool conclusionRepetida(std::string valor , std::vector<std::string> conclusion)
    {
        for (unsigned int i = 0; i < conclusion.size(); i++) {
            if (conclusion[i] == valor)
                return true;
        }
        return false;
    }
    
    // Algoritmo BFS para forward chaining
    bool obtenConsecuenciaForward(Nodo* raiz)
    {
        if (!raiz)
            return false;
        std::queue<Nodo*> q;
        std::vector<std::string> conclusion;
        bool conclusionValida = false;
        unsigned int i;
        q.push(raiz);  
        while (!q.empty())
        {
            int n = q.size(); 
            while (n > 0)
            {
                Nodo* p = q.front();
                q.pop();
                for (i = 0; i < p->hijo.size(); i++){
                    if (p->fueRechazado){
                        p->hijo[i]->fueRechazado = true;
                    }
                    q.push(p->hijo[i]);                    
                }
                if (p->hijo.size() == 0 && !(p->fueRechazado)) {
                    if (!conclusionRepetida(p->valor,conclusion)){
                        conclusion.push_back(p->valor);
                        conclusionValida = true;
                    }
                }
                n--;
            }
        }
        if(conclusion.size()>1){
            std::cout << "Resultado: Desconocido (No existe resultado que calce con TODAS las caracteristicas entregadas)" << std::endl;
        }
        else{
            std::cout << "Resultado: "<< std::endl;
            for (i = 0; i < conclusion.size(); i++) {
                std::cout << conclusion[i] << std::endl;
            }
        }
        return conclusionValida;
    }

    void eliminaNodoDeRaiz(Nodo *condicion, Nodo *raiz)
    {
        bool nodoEliminado = false;
        for (unsigned int i = 0; i < (raiz->hijo).size(); i++) {
            if (nodoEliminado == false) {
                if (raiz->hijo[i]->valor == condicion->valor) {
                    (raiz->hijo).erase( (raiz->hijo).begin() + i);
                    nodoEliminado = true;
                }
            }
        }    
    }

    bool nodoRepetido(Nodo* raiz, std::string condicion)
    {
        for (unsigned int i = 0; i < (raiz->hijo).size(); i++) {
            if (raiz->hijo[i]->valor == condicion) {
                return true;
            }
        }
        return false;
    }

    void agregaCondicionANodo(Nodo* condicion, std::string consecuencia, Nodo* raiz)
    {
        bool nodoPosicionado = false;
        for (unsigned int i = 0; i < (raiz->hijo).size(); i++) {
            if (nodoPosicionado == false) {
                if (raiz->hijo[i]->valor == consecuencia) {
                    if (!nodoRepetido(raiz->hijo[i], condicion->valor)) {
                        raiz->hijo[i]->esHoja = false;
                        (raiz->hijo[i]->hijo).push_back(condicion);
                    }                    
                    nodoPosicionado = true;
                }
                else {
                    agregaCondicionANodo(condicion, consecuencia, raiz->hijo[i]);
                }
            }
        }
    }

    void agregaCondicionANodoForward(Nodo* condicion, Nodo* consecuencia, Nodo* raiz)
    {
        bool nodoPosicionado = false;
        for (unsigned int i = 0; i < (raiz->hijo).size(); i++) {
            if (nodoPosicionado == false) {
                if (raiz->hijo[i]->valor == condicion->valor) {
                    raiz->hijo[i]->esHoja = false;
                    (raiz->hijo[i]->hijo).push_back(consecuencia);
                    nodoPosicionado = true;
                }
                else {
                    agregaCondicionANodoForward(condicion, consecuencia, raiz->hijo[i]);
                }
            }
        }
    }

    void recorreArbol(Nodo* p, std::ofstream& outfile)
    {
        char comilla = '"';
        std::string padre = p->valor;
        for (unsigned int i = 0; i < p->hijo.size(); i++)
        {
            outfile << comilla << padre << comilla << "->";
            outfile << comilla << p->hijo[i]->valor << comilla << std::endl;
            recorreArbol(p->hijo[i], outfile);
        }
    }

    void hojasRankSame(Nodo* raiz, std::ofstream& outfile)
    {
        for (unsigned int i = 0; i < (raiz->hijo).size(); i++) {
            if (raiz->hijo[i]->esHoja) {
                outfile << '"' << raiz->hijo[i]->valor << '"' << ";";
            }
            else {
                hojasRankSame(raiz->hijo[i], outfile);
            }
        }
    }

    void generaGraficoArbol(Nodo* raiz, std::string tipoEncadenamiento)
    {
        if (!raiz)
            return;
        char comilla = '"';
        std::ofstream outfile("sistema_experto_arbol.dot");
        outfile << "strict digraph G {" << std::endl;
        outfile << "graph [ordering="<< comilla << "out" <<comilla<<"];" << std::endl;
        outfile << "graph [ranksep=" << comilla << "1.5" << comilla << "];" << std::endl;
        recorreArbol(raiz, outfile);
        outfile << "{ rank = same;";
        for (unsigned int i = 0; i < raiz->hijo.size(); i++)
        {
            outfile << comilla << raiz->hijo[i]->valor << comilla << ";";
        }
        outfile << "}\n";
        if (tipoEncadenamiento == "forward") 
        {
            outfile << "{ rank = same;";
            hojasRankSame(raiz, outfile);
            outfile << "}" << std::endl;
        }
        outfile << "}" << std::endl;
        outfile.close();
    }    

    // Algoritmo DFS para backward chaining
    bool esEstaConsecuenciaInput(Nodo* nodo, std::vector<std::string> input)
    {
        int respuesta = 0;
        unsigned int i = 0;
        //comprueba rechazo previo
        for (i = 0; i < (nodo->hijo).size(); i++)
        {
            if (nodo->hijo[i]->fueRechazado) {
                return false;
            }
        }
        for (i = 0; i < (nodo->hijo).size(); i++)
        {
            if (!esEstaConsecuenciaInput(nodo->hijo[i],input)) {
                nodo->fueRechazado = true;
                return false;
            }
        }
        if (nodo->esHoja) //o que es lo mismo, el nodo es hoja i == 0 
        {
            if (nodo->fueAceptado)
                return true;
            if (nodo->fueRechazado)
                return false;
            for (unsigned int i = 0; i < input.size(); i++) {
                if (input[i] == nodo->valor) {
                    nodo->fueAceptado = true;
                    return true;
                }
            }
            nodo->fueRechazado = true;
            return false;
        }
        //pasamos todos los filtros, es decir, tiene hijos y no es un nodo hoja
        return true;
    }

}

#endif
