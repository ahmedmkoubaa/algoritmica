#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <cmath>
#include "limits.h"

#include <chrono>
#include <cstring>
#include <random>


#define INFINITO 1<<64-1
using namespace std;
using namespace std::chrono;

struct Ciudad {double id=0, x=0, y=0;};

// Función para notificar error y abortar el programa
void Error(const char * msg){
	cerr << "ERROR: " << msg << endl;
	cerr << "USO: ./<nombre programa> <numero de ciudades> [nombre fichero]" << endl;
	exit(EXIT_FAILURE);
}

/******************************************************************************/
// Devuelve la distancia euclidiana entre una ciudad y otra
double Distancia(const Ciudad & origen, const Ciudad & destino){
	double dx = destino.x - origen.x;			// Distancia en el eje x
	double dy = destino.y - origen.y;			// Distancia en el eje y
	return sqrt(dx*dx + dy*dy);						// Raiz de la suma de las diferencias al cuadrado
}


void ImprimeMatriz(const char * msg, const vector<vector<double>> & matriz){
	const int DIM = matriz.size();

	cout << msg;
  for (auto v: matriz){
    for (auto d: v) cout << d << " ";
    cout << endl;
  }
	cout << endl << endl;
}


/******************************************************************************/
// Genera un vector de ciudades con el tamaño indicado
// El formato del fichero esperado es 3 numeros por linea, donde
// el primero es el id de la ciudad y los otros sus coordenadas x e y
vector<Ciudad> GenerarCiudades(const int DIM, const char * nombre){
	ifstream fi(nombre);
	if (!fi){Error("No se pudo abrir el fichero indicado");}

	double id, x, y, i = 0;
	vector<Ciudad> ciudades; Ciudad ciudad;

	while (!fi.eof() && i < DIM){
		fi >> id >> x >> y;
		ciudad.id = id; ciudad.x = x; ciudad.y = y;
		ciudades.push_back(ciudad);
		++i;
	}

	fi.close();
	return ciudades;
}

// vector<vector<double>> gm;
/******************************************************************************/
// Genera una matriz en la que cada elemento se corresponde a la distancia
// entre la ciudad j y la ciudad i (siendo estos índices de fila y columnas)
vector<vector<double>> GenerarMatrizDistancias(const vector<Ciudad> & ciudades){
	const int DIM = ciudades.size();
	vector<vector<double>> distancias(DIM);
	for (int i = 0; i < DIM; ++i) distancias[i] = vector<double>(DIM);

	for (int i = 0; i < DIM; ++i)
		for (int j = 0; j < DIM; ++j)
			distancias[i][j] = Distancia(ciudades[i], ciudades[j]);

	return distancias;
}


/******************************************************************************/
double TSP(const vector<vector<double>> & L, int ciu_actual, int vistas, vector<vector<double>> & min_path){
	double minimo;
	if (vistas != ((1 << L.size()) - 1)){
		if (min_path[ciu_actual][vistas] == INT_MAX){
			double distancia = minimo = INT_MAX;
			const int size = L.size();

			for (int i = 0; i < size; ++i){
				if (i == ciu_actual || (1<<i) & vistas) continue;
				distancia = L[ciu_actual][i] + TSP(L, i, vistas|(1<<i), min_path);
				if (distancia < minimo) minimo = distancia;
			}

			min_path[ciu_actual][vistas] = minimo;
		}
		else {
			minimo = min_path[ciu_actual][vistas];
		}
	}
	else {
		minimo = L[ciu_actual][0];		// Ya fueron visitadas todas las ciudades enteras
	}

	return minimo;
}

int main(int argc, char ** argv)
{
  if (argc < 2){Error("Número de argumentos incorrectos");}
  const int DIM = atoi(argv[1]);

  vector<Ciudad> ciudades = GenerarCiudades(DIM, argv[2]);
  vector<vector<double>> cities = GenerarMatrizDistancias(ciudades);
  // ImprimeMatriz("Distancias entre ciudades\n", cities);

	vector<vector<double>> min_dist(cities.size());
	for(auto& neighbors : min_dist)
			neighbors = vector<double>((1 << cities.size()) - 1, INT_MAX);

	high_resolution_clock::time_point tantes, tdespues;
	duration<double> transcurrido;
	tantes = high_resolution_clock::now();

	double dist = TSP(cities, 0, 1, min_dist);

	tdespues = high_resolution_clock::now();
	transcurrido = duration_cast<duration<double>>(tdespues - tantes);
	cout << DIM << " " << transcurrido.count() << endl;

	cout << "Minimo: " << dist << endl;



  return 0;
}
