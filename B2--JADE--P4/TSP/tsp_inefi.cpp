#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <cstring>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <limits.h>

#include <chrono>
#include <cstring>
#include <random>

#define INFINITO 239712317013120
using namespace std;
using namespace std::chrono;

// #define INFINITO 18446744073709551615.0 // Número mas grande que se puede almacenar en un double

/******************************************************************************/
// Estructura para representar las ciudades, se compone de coordenada x e y y un id
struct Ciudad {double id=0, x=0, y=0;};

/******************************************************************************/
// Función para notificar error y abortar el programa
void Error(const char * msg);

/******************************************************************************/
// Generar aleatoriamente las ciudades reprsentadas por sus coordenadas
vector<Ciudad> GenerarCiudades(const int DIM);

/******************************************************************************/
// Genera un vector de ciudades con el tamaño indicado
vector<Ciudad> GenerarCiudades(const int DIM, const char * nombre);

/******************************************************************************/
// Muestra todas las coordenadas de las ciudades de manera enumerada
void ImprimeCiudades(const char * msg, const vector<Ciudad> & ciudades);

/******************************************************************************/
// Devuelve la secuencia de ciudades que se obtiene al recorrer todas
// las ciudades pasadas desde la primera hasta la ultima volviendo a la primera
void RecorreCiudades(Ciudad * ciudades, int & num_ciudades);

/******************************************************************************/
// Devuelve la distancia euclidiana entre una ciudad y otra
double Distancia(const Ciudad & origen, const Ciudad & destino);

/******************************************************************************/
// Genera un entero aleatorio
int aleatorio(int min, int max);

/******************************************************************************/
// Devuelve la distancia total recorrida para hacer el camino
double DistanciaCamino(const Ciudad * ciudades, const int num_ciudades);

/******************************************************************************/
// Genera una matriz en la que cada elemento se corresponde a la distancia
// entre la ciudad j y la ciudad i (siendo estos índices de fila y columnas)
vector<vector<double>> GenerarMatrizDistancias(const vector<Ciudad> & ciudades);

/******************************************************************************/
// Devuelve una matriz cuadrada de dimension DIM inicializada a -1 como valor
// nulo (se podria usar el cero pero es una distancia)
vector<vector<double>> GenerarMatrizMinima(const int DIM);

/******************************************************************************/
// Recorre todos los elementos de la matriz mostrandolos por pantalla
void ImprimeMatriz(const char * msg, const vector<vector<double>> & matriz);


/******************************************************************************/
// Funcion recursiva que usa la memoria para guardar datos sobre la distancia
// en la seunda matriz y encontrar asi la distancia minima
double g(const int i, vector<int> s,
		const vector<vector<double>> & L, map<string, double> & gtab){

	double mas_corto, distancia;
	int pos_min;

	if (s.empty()){		// Vector vacío
		return L[i][0];	// Volver a ciudad de inicios
	}
	else {
		ostringstream oss; oss << i << " ";		// Introducir ciudad actual

		for (int ciudad: s) oss << " " << ciudad;
		string recorridas = oss.str();

	 	double encontrado = gtab[recorridas];
		if (encontrado) return encontrado;

		// Si no se salio, entonces estamos en el else y habrá que buscar una ruta más corta
		mas_corto = INFINITO;
		const int tam = s.size();		// Número de ciudades a inspeccionar
		int sig_ciu;								// Para guardar el indice de la ciudad siguiente

		for (int j = 0; j < tam; ++j){
				sig_ciu = s[j];														// Sacar ciudad siguiente del conjunto
				s.erase(s.begin()+j);											// Eliminarla para ello

				distancia = L[i][sig_ciu] + g(sig_ciu, s, L, gtab);		// Buscar nuevo minimo para ello
				s.insert(s.begin()+j, sig_ciu);																// Volverlo a insertar

				if (distancia < mas_corto) mas_corto = distancia;
		}

		gtab[recorridas] = mas_corto;
	}
}

/******************************************************************************/
int main (int argc, char ** argv){
	if (argc < 2){Error("Número de argumentos incorrectos");}

	const int DIM = atoi(argv[1]);

	//--------------------------------------------------------------------------//
	// Generar conjuto de ciudades a recorrer
	vector<Ciudad> ciudades =	GenerarCiudades(DIM, argv[2]);
	vector<vector<double>> distancias = GenerarMatrizDistancias(ciudades);
	// ImprimeMatriz("", distancias);

	map<string, double> distancias_min;
	vector<int> s;
	for (int i = 1; i < DIM; ++i) s.push_back(i);

	list<int> res;
	high_resolution_clock::time_point tantes, tdespues;
	duration<double> transcurrido;
	tantes = high_resolution_clock::now();

	const double dist = g(0,s, distancias, distancias_min);

	tdespues = high_resolution_clock::now();
	transcurrido = duration_cast<duration<double>>(tdespues - tantes);
	cout << DIM << " " << transcurrido.count() << endl;

	cout << "Minimo: " << dist << endl;
	return 0;
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

/******************************************************************************/
// Generar aleatoriamente las ciudades reprsentadas por sus coordenadas
vector<Ciudad> GenerarCiudades(const int DIM){
	const int MIN = 0, MAX = 1000;
	vector<Ciudad> ciudades(DIM);
	for (int i = 0; i < DIM; ++i){
		ciudades[i].id = i;											// Asignar id de la ciudad
		ciudades[i].x  = aleatorio(MIN, MAX);		// Asignar eje x aleatorio
		ciudades[i].y  = aleatorio(MIN, MAX);		// Asignar eje y aleatorio
	}

	return ciudades;
}

/******************************************************************************/
// Devuelve una matriz cuadrada de dimension DIM inicializada a -1 como valor
// nulo (se podria usar el cero pero es una distancia)
vector<vector<double>> GenerarMatrizMinima(const int DIM){
	vector<vector<double>> minimos(DIM);
	for (int i = 0; i < DIM; ++i)
		for (int j = 0; j < DIM; ++j)
			minimos[i].push_back(-1);
	return minimos;
}

/******************************************************************************/
// Genera un entero aleatorio
int aleatorio(int min, int max){
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

/******************************************************************************/
// Muestra las ciudades en el orden de la coleccion pasada, muestra id y coordenadas
void ImprimeCiudades(const char * msg, const vector<Ciudad> & ciudades){
	int num_ciudades = ciudades.size();

	cout << msg;
	for (int i = 0; i < num_ciudades; ++i)
		cout << ciudades[i].id << " " << ciudades[i].x << " " << ciudades[i].y << endl;
	cout << endl << endl;
}

/******************************************************************************/
// Recorre todos los elementos de la matriz mostrandolos por pantalla
void ImprimeMatriz(const char * msg, const vector<vector<double>> & matriz){
	const int DIM = matriz.size();

	cout << msg;
	for (int i = 0; i < DIM; ++i){
		for (int j = 0; j < DIM; ++j){
			cout << matriz[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

/******************************************************************************/
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

/******************************************************************************/
// Devuelve la distancia total recorrida para hacer el camino
// Las ciudades son una secuencia ordenada que representa el camino
double DistanciaCamino(const Ciudad * ciudades, const int num_ciudades){
	double suma = 0;
	int size = num_ciudades-1;

	for (int i = 0; i < size; ++i)
		suma += Distancia(ciudades[i], ciudades[i+1]);

	return suma;
}
