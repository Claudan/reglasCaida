#include "ReglaSemantica.h"
#include <string>
#include <iostream>

namespace ExpertoLib {

	ReglaSemantica::ReglaSemantica(std::string val, std::string arc, std::string hij) {
		valor = val;
		arco = arc;
		hijo = hij;
	}

	/* Getters */
	std::string ReglaSemantica::getValorPadre() {
		return valor;
	}

	std::string ReglaSemantica::getValorArco() {
		return arco;
	}

	std::string ReglaSemantica::getValorHijo() {
		return hijo;
	}
}