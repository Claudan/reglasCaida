#include "Regla.h"
#include <vector>
#include <string>
#include <iostream>

namespace ExpertoLib {

		Regla::Regla(std::vector<std::string> cond, std::string cons) 
		{
			condiciones = cond;
			consecuencia = cons;
			comentario = " ";
		}

		Regla::Regla(std::vector<std::string> cond, std::string cons, std::string com) 
		{
			condiciones = cond;
			consecuencia = cons;
			comentario = com;
		}

		/* Getters */
		std::vector<std::string> Regla::getCondiciones() 
		{
			return condiciones;
		}

		std::string Regla::getConsecuencia() 
		{
			return consecuencia;
		}

		std::string Regla::getComentario() 
		{
			return comentario;
		}

		/* Setters */
		void Regla::setComentario(std::string comentario) 
		{
			comentario = comentario;
		}

}