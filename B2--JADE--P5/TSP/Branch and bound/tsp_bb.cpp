#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <fstream>
#include <cmath>
#include "limits.h"

#include <chrono>
#include <cstring>
#include <random>

using namespace std;
using namespace std::chrono;

const double INFINITO = 1000000000000000000;

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

struct Nodo{
	int ciu;
	double ac = 0;
	double costo_posible = 0;
	list<int> secuencia;
	vector<int> ciudades;
};

/***********************************************************************************************************************/
// Para la cola con prioridad
bool operator < (const Nodo & m1, const Nodo & m2){
	return m1.costo_posible> m2.costo_posible;
}

long int TAM_MAX = 0;
long int EXPANDIDOS = 0;
long int PODADOS = 0;

void EstimarCoste(Nodo & actual, priority_queue<Nodo> & cola,  const vector<vector<double>> & D){
	vector<int> ciudades = actual.ciudades;
	double min, dist_sig;

	for (int i = 0; i < ciudades.size(); ++i){

		Nodo nuevo = actual;
		nuevo.ciu = ciudades[i];
		nuevo.ac += D[actual.ciu][nuevo.ciu];

		int sig_ciu = ciudades[i];
		ciudades.erase(ciudades.begin()+i);


		// Buscar la ciudad minima para la ciudad siguiente
		min = INFINITO;
		for (int j = 0; j < ciudades.size(); ++j){
			dist_sig = D[sig_ciu][ciudades[j]];
			if (dist_sig < min) min = dist_sig;
		}

		nuevo.costo_posible =  nuevo.ac;
		nuevo.costo_posible += min;
		min 		 =  INFINITO;

		ciudades.push_back(0); 	// añadir la ciudad de retorno

		for (int j = 0; j < ciudades.size()-1; ++j){
			min = INFINITO;

			for (int ciudad: ciudades){
				dist_sig = D[ciudades[j]][ciudad];
				if (dist_sig < min && dist_sig) min = dist_sig;
			}

			// cout << "el minimo encontrado es:" << min << endl;
			nuevo.costo_posible += min;
		}


		ciudades.pop_back();
		nuevo.ciudades = ciudades;
		ciudades.insert(ciudades.begin()+i, sig_ciu);
		cola.push(nuevo);
	}

	if (cola.size() > TAM_MAX) TAM_MAX = cola.size();
	++EXPANDIDOS;
}

double TSP2(Nodo & origen, double & coste_minimo, list<int> & recorrido, const vector<vector<double>> & D){
	priority_queue<Nodo> cola;
	Nodo hijo;

	cola.push(origen);

	while (!cola.empty()){
		hijo = cola.top(); cola.pop();
		if (hijo.ac > coste_minimo) {
			PODADOS = cola.size();
			return coste_minimo;
		}

		hijo.secuencia.push_back(hijo.ciu);
		EstimarCoste(hijo, cola, D);

		if (hijo.ciudades.size() == 1){
			hijo.ac += D[hijo.ciu][hijo.ciudades[0]] + D[hijo.ciudades[0]][0];
			// cout << "Encontrada una solución " << actual.ac << endl;
			if (hijo.ac < coste_minimo){
				coste_minimo = hijo.ac;
				hijo.secuencia.push_back(hijo.ciudades[0]);
				hijo.secuencia.push_back(0);
				recorrido = hijo.secuencia;
			}
		}
	}

	return coste_minimo;
}

double TSP(Nodo & actual, vector<int> ciudades, double & coste_minimo, list<int> & recorrido, const vector<vector<double>> & D){
	// Recibir la ciudad actual y las ciudades a visitar aún
	// Para toda ciudad a visitar calcular el acumulado y el costo_posible
	// Introducir todas las ciudades en una cola con prioridad y e ir sacandolas.

	actual.secuencia.push_back(actual.ciu);
	if (ciudades.size() == 1){
		actual.ac += D[actual.ciu][ciudades[0]] + D[ciudades[0]][0];
		// cout << "Encontrada una solución " << actual.ac << endl;
		if (actual.ac < coste_minimo){
			coste_minimo = actual.ac;
			actual.secuencia.push_back(ciudades[0]);
			actual.secuencia.push_back(0);
			recorrido = actual.secuencia;
		}


		return coste_minimo;
	}

	priority_queue<Nodo> cola;
	// cout << endl << "El actual es: " << actual.ciu << endl;
	// Vamos a actualizar todos los costes de la expansión de los nodos
	const int size = ciudades.size();
	double dist_sig, min;
	double costo_posible = 0;

	// Calcular el coste posible para cada nodo e introducirlo a
	// cola con prioridad para luego recorrerlo
	actual.ciudades = ciudades;
	EstimarCoste(actual, cola, D);

	Nodo sig_nodo;
	for (int i = 0; i < cola.size(); ++i){
		sig_nodo = cola.top(); cola.pop();
		if (sig_nodo.costo_posible >= coste_minimo) return coste_minimo;

		int pos;
		for (int j = 0; j < size; ++j){
			if (ciudades[j] == sig_nodo.ciu){
				pos = j; break;
			}
		}

		ciudades.erase(ciudades.begin()+pos);
		TSP(sig_nodo, ciudades, coste_minimo, recorrido, D);
		ciudades.insert(ciudades.begin()+pos, sig_nodo.ciu);
	}
}

int main(int argc, char ** argv)
{
  if (argc < 2){Error("Número de argumentos incorrectos");}
  const int DIM = atoi(argv[1]);

  vector<Ciudad> ciudades = GenerarCiudades(DIM, argv[2]);
  vector<vector<double>> cities = GenerarMatrizDistancias(ciudades);
  // ImprimeMatriz("Distancias entre ciudades\n", cities);

	Nodo origen;
	vector<int> indices;


	origen.ciu = ciudades[0].id-1;
	origen.costo_posible = origen.ac = 0;
	for (int i = 1; i < ciudades.size(); ++i)
		indices.push_back(ciudades[i].id-1);

	double coste_minimo = INFINITO;
	list<int> recorrido;


	high_resolution_clock::time_point tantes, tdespues;
	duration<double> transcurrido;
	tantes = high_resolution_clock::now();

	origen.ciudades = indices;

	TSP2(origen, coste_minimo, recorrido, cities);
	// TSP(origen, indices, coste_minimo, recorrido, cities);

	tdespues = high_resolution_clock::now();
	transcurrido = duration_cast<duration<double>>(tdespues - tantes);

	cout << DIM << " " << transcurrido.count()
			 << " " << TAM_MAX << " " << EXPANDIDOS << " " << PODADOS << endl;


	for (int ci: recorrido) cout << ci << " ";
	cout << endl;

	for (int ci: recorrido)
		cout << ciudades[ci].id << " " << ciudades[ci].x
				 << " " << ciudades[ci].y << " " << endl;
	// cout << endl;
	//
	// cout << TAM_MAX << " Nodos expandidos: " << EXPANDIDOS
	// 		 << " nodos podados: " << PODADOS << endl;

  return 0;
}
