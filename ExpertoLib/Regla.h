#ifndef Regla_H
#define Regla_H

#include <vector>
#include <string>
#include <iostream>

namespace ExpertoLib {

        class Regla
        {
        private:
            std::vector<std::string> condiciones;
            std::string consecuencia;
            std::string comentario;

        public:
            Regla(std::vector<std::string> condiciones, std::string consecuencia);
            Regla(std::vector<std::string> condiciones, std::string consecuencia, std::string comentario);
            void setComentario(std::string comentario);
            std::vector<std::string> getCondiciones();
            std::string getConsecuencia();
            std::string getComentario();
        };

}

#endif