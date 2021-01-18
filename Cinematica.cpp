#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include "ExpertoLib.h"

using namespace std;
using namespace cv;
using namespace ExpertoLib;

//Método de eliminación de Gauss
void eliminacionGauss(double** A, double B[], int n) {
	double inv;
	for (int k = 0; k < n; k++) {
		for (int i = k + 1; i < n; i++) {
			inv = A[i][k] / A[k][k];
			for (int j = k; j < n; j++) {
				A[i][j] = A[i][j] - inv * A[k][j];
			}
			B[i] = B[i] - inv * B[k];
		}
	}
}

//Gauss: sustitución inversa.
void sustitucionAtras(double** A, double B[], int n, double C[]) {
	double suma;
	C[n - 1] = B[n - 1] / A[n - 1][n - 1];
	for (int i = n - 2; i >= 0; i--) {
		suma = 0;
		for (int j = i + 1; j < n; j++) {
			suma = suma + A[i][j] * C[j];
		}
		C[i] = (B[i] - suma) / A[i][i];
	}
}

//Coeficientes de la regresión polinomial de grado 2 -> A0, A1, A2
float* coeficientes(vector<double> x, vector<double> y, int n) {
	double sum_x = 0, sum_xy = 0;
	int tamano = 3; //grado + 1
	double* solucion = new double[tamano];
	double** ecuaciones;

	ecuaciones = new double* [tamano];
	//Inicialización del arreglo bidimensional.
	for (int i = 0; i < tamano; i++) {
		ecuaciones[i] = new double[tamano];
	}

	//Cálculo de las sumatorias y armado del sistema.
	for (int i = 0; i < tamano; i++) {
		sum_xy = 0;

		for (int j = 0; j < n; j++)
			sum_xy += pow(x[j], i) * y[j];
		solucion[i] = sum_xy;

		for (int j = 0; j < tamano; j++) {
			sum_x = 0;
			if (i == 0 && j == 0)
				ecuaciones[i][j] = n;
			else {
				for (int h = 0; h < n; h++)
					sum_x += pow(x[h], (j + i));
				ecuaciones[i][j] = sum_x;
			}
		}
	}

	//Resolucion de sistemas de ecuaciones.
	eliminacionGauss(ecuaciones, solucion, tamano);

	double* x_vector = new double[tamano];

	sustitucionAtras(ecuaciones, solucion, tamano, x_vector);

	//Coeficientes resultantes
	float* coeficientes = (float*)malloc(sizeof(float) * 3);
	coeficientes[0] = (float)x_vector[0];  // A0 
	coeficientes[1] = (float)x_vector[1];  // A1
	coeficientes[2] = (float)x_vector[2];  // A2 este se multiplica x2 para calcular la aceleracion
	return coeficientes;
}

//Retorna la aceleración y la velocidad de los datos en el intervalo actual (14frames = 0.55 [s])
float* calculaCinematica(vector<double> posicionY, int intervalo_frames, double altura_bb_promedio, double segundos_por_frame, double cabeza_promedio) {

	vector<double> posicionYEnMetros; // ejemplo: [0, -1.3, -1.4,   -3, ...], esto es en metros
	vector<double> tiempo;            // ejemplo: [0, 0.04, 0.08, 0.12, ...], esto es en segundos         

	for (int i = 0; i < posicionY.size(); i++) {

		//Tomamos como Y0 = 0, es decir, la primera altura del intervalo actual será nuestra referencia
		double posicionReferenciada = posicionY[0] - posicionY[i];

		//Transformamos [px] a [m] 
		//proporción entre el bounding box promedio en [px] y una cabeza de adulto promedio en [m]
		posicionYEnMetros.push_back((posicionReferenciada * cabeza_promedio) / altura_bb_promedio);

		//El primer tiempo del intervalo actual también parte de 0[s] independiente del tiempo del video
		tiempo.push_back(i * segundos_por_frame);
	}

	//Cinemática:           y(t) = y0 + v0*t - (1/2)*g*t2 
	//Regresión actual:     y(x) = A0 + A1x  + A2*x2 
	//==> segunda derivada: y''(x) = 2*A2 (esto es la aceleración)
	
	return coeficientes(tiempo, posicionYEnMetros, intervalo_frames);

}


vector<double> getVerticalPosition(string ruta) {

	vector<double> eje_y;
	ifstream bbFile;

	bbFile.open(ruta);

	while (bbFile.good()) {

		string line;
		//getline(bbFile, line, ',');

		getline(bbFile, line, '\n');

		eje_y.push_back(stoi(line));
	}
	return eje_y;

}

double promedioVector(vector<double> alturasBB, int inicio, int intervalo_frames) {

	double sumaTotal = 0;
	int fin = inicio + intervalo_frames;
	for (int i = 0; i < alturasBB.size(); i++) {
		if (i >= inicio && i < fin) {
			sumaTotal += alturasBB[i];
		}
	}

	return sumaTotal / alturasBB.size();
}

string numberToStringPath(int numero) {

	if (numero < 10) {
		return "0000" + to_string(numero);
	}
	if (numero < 100) {
		return "000" + to_string(numero);
	}
	if (numero < 1000) {
		return "00" + to_string(numero);
	}
	if (numero < 10000) {
		return "0" + to_string(numero);
	}
	return to_string(numero);
}

Experto conocimientoExpertoCaidas()
{
	Experto exp = Experto();

	vector<string> condiciones;
	string consecuencia;

	//Determinación de caída libre
	consecuencia = "Caida Libre";
	condiciones = { "[aceleracion] cercana a -9.8 [m/s2]" };
	exp.insertaRegla(Regla(condiciones, consecuencia));

	//Peligrosidad de la caida
	consecuencia = "Caida de Peligrosidad Alta";
	condiciones = { "Caida Libre", "[velocidad] >= 0.5 [m/s]"};
	exp.insertaRegla(Regla(condiciones, consecuencia));

	//Peligrosidad de la caida
	consecuencia = "Caida de Peligrosidad Moderada";
	condiciones = { "Caida Libre", "[velocidad] < 0.5 [m/s]" };
	exp.insertaRegla(Regla(condiciones, consecuencia));
	//exp.generaGrafico();
	
	//Agregar más reglas asociadas a las fases de la caída
	/*
	a) Inicio de la caída
	b) Caída libre
	c) Deceleración
	d) Rebote
	e) Suspensión
	
	*/
	return exp;
}

void imprimeInferencias(vector<string> solucionesexp) {
	for (int i = 0; i < solucionesexp.size(); i++) {
		cout << "Inferencia Actual: [ " << solucionesexp.at(i) << " ]" << endl;
	}
	cout << endl;
}

int main() {
	/* Sistema Experto sobre caídas y su peligrosidad */
	Experto expertoCaidas;
	string aceleracionActual;     //Variable (condición de la regla)
	string velocidadActual;       //Variable (condición de la regla)
	vector<string> solucionesexp; //Resultado de la inferencia

	vector<double> tiempo;
	vector<double> posicionY;
	vector<double> registroPosicionesY;
	float* coeficientesRegresion2;
	float aceleracion;
	float velocidad;

	double segundos_por_frame = 0.04; // esto a 25 [FPS]	
	int intervalo_frames = 13;//13 frames equivalen a 0.53s (caida libre de 1.6m) en un video de 25fps
	double bb_promedio;

	int    frame_actual = 0;
	double tiempo_actual = 0;
	double posicion_actual = 0;
	double cabeza_promedio = 0.225; //cabeza adulta promedio en metros
	double aceleracionDeGravedad = -9.8; //aceleración en [m/s2]
	double filtroCercanas = 0.35; //porcentaje de aceleraciones cercanas 

	int numEjemplo = 1; //numero de caida
	registroPosicionesY = getVerticalPosition("Recursos/caida"+ to_string(numEjemplo) +"/caida.csv");

	//*Alturas de los Bounding_box en cada frame
	//vector<double> alturasBB = getAlturasBB("Recursos/caida1/caida.csv");

	string path = "Recursos/caida" + to_string(numEjemplo) + ".mp4";
	VideoCapture cap(path);
	Mat img;

	int total_frames = cap.get(CAP_PROP_FRAME_COUNT);

	while (frame_actual < total_frames) {

		//Referencia para ubicación en el Video
		tiempo_actual = frame_actual * segundos_por_frame;
		posicion_actual = registroPosicionesY[frame_actual];
		frame_actual += 1;

		if (posicionY.size() >= intervalo_frames) {

			//Eliminamos el primer elemento insertado
			posicionY.erase(posicionY.begin());

			//Agregamos el elemento actual
			posicionY.push_back(posicion_actual);

			//*Promedio de alturas de bounding_box en el rango medido, modificar este punto cuando tengamos YOLO
			//double bb_promedio = promedioVector(alturasBB, frame_actual, intervalo_frames);

			//*Promedio de alturas de bounding_box (temporal)
			bb_promedio = 46;

			//calculamos aceleración y velocidad en el intervalo de frames dado (Regresión polinomial de grado 2 y derivadas)
			float* cinematica = calculaCinematica(posicionY, intervalo_frames, bb_promedio, segundos_por_frame, cabeza_promedio);
			aceleracion = 2*cinematica[2]; //==> segunda derivada: y''(x) = 2*A2 (esto es la aceleración)
			velocidad   = cinematica[1];
			if (aceleracion < aceleracionDeGravedad * (1 - filtroCercanas)) {
				//Datos actuales 
				cout << "-------Datos Actuales-------: " << endl;
				//cout << "Regresion: "<< cinematica[1] <<" + " << cinematica[1] << "*x + " << cinematica[2]<<"*x2"<< endl;
				cout << "aceleracion: " << aceleracion << " [m/s2]" << endl;
				cout << "velocidad: " << velocidad << endl;
				//cout << "frame_actual: " << frame_actual << endl;
				
				//Variables actuales para el S.E.
				aceleracionActual = "[aceleracion] cercana a -9.8 [m/s2]";
				if (abs(velocidad) >= 0.5) { //3
					velocidadActual = "[velocidad] >= 0.5 [m/s]";
				}
				else {
					velocidadActual = "[velocidad] < 0.5 [m/s]";
				}

				//Inferencia S.E. a partir del dato actual
				Experto exp = conocimientoExpertoCaidas();
				solucionesexp = exp.infiereBackwardChaining({ aceleracionActual, velocidadActual });
				imprimeInferencias(solucionesexp);
			}

		}
		else {
			posicionY.push_back(posicion_actual);
		}
		cap.read(img);
		imshow("Camara", img);
		waitKey(27);

	}


	return 0;
}

