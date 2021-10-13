#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <cstring>
#include <random> // dispositivos, generadores y distribuciones aleatorias

using namespace std;
using namespace std::chrono;

#define MAX_BUSQUEDA 100000000000.0
/******************************************************************************/
// Estructura para representar las ciudades, se compone de coordenada x e y y un id
struct Ciudad {double id=0, x=0, y=0, distancia=0;};

/******************************************************************************/
// Función para notificar error y abortar el programa
void Error(const char * msg);

/******************************************************************************/
// Generar aleatoriamente las ciudades reprsentadas por sus coordenadas
void GenerarCiudades(Ciudad * ciudades, const int DIM);

/******************************************************************************/
// Genera un vector de ciudades con el tamaño indicado
void GenerarCiudades(Ciudad * ciudades, const int DIM, const char * nombre);

/******************************************************************************/
// Muestra todas las coordenadas de las ciudades de manera enumerada
void ImprimeCiudades(const char * msg, const Ciudad * ciudades, const int num_ciudades);

/******************************************************************************/
// Devuelve la secuencia de ciudades que se obtiene al recorrer todas
// las ciudades pasadas desde la primera hasta la ultima volviendo a la primera
void RecorreCiudades(Ciudad * ciudades, int & num_ciudades);

/******************************************************************************/
// Devuelve la distancia euclidiana entre una ciudad y otra
double Distancia(const Ciudad & origen, const Ciudad & destino);

/******************************************************************************/
// Llama al algoritmo de ordenacion con los parametros adecuados
void OrdenaCiudades(Ciudad * ciudades, const int num_ciudades);

/******************************************************************************/
// Ordena por mergesort el vector de ciudades pasado
// el criterio de orden son las distancias
void OrdenaCiudades(Ciudad * v, int inicio, int final);

/******************************************************************************/
// Genera un entero aleatorio
int aleatorio(int min, int max);

/******************************************************************************/
// Devuelve la distancia total recorrida para hacer el camino
double DistanciaCamino(const Ciudad * ciudades, const int num_ciudades);

/******************************************************************************/
// Busca la ciudad mas lejana desde la ciudad origen
// Si no la encuentra devuelve -1, en otro caso devuelve otra posicion
int BuscaMaxDistancia(const Ciudad & origen, const Ciudad * ciudades, const int size);

/******************************************************************************/
// Elimina la ciudad que se encuentre en la posicion indicada
void EliminarCiudad(const int pos, Ciudad * ciudades, int & size);

/******************************************************************************/
// Variación del algoritmo que sigue la heuristica del vecino más cercano
// inspirandose del algoritmo A* (estrella), usado en IA para busqueda de camino
Ciudad * RecorreCiudadesA(Ciudad * caminoo, int & num_ciudades);

/******************************************************************************/
int main (int argc, char ** argv){
	if (argc < 2){Error("Número de argumetos incorrectos");}

	int num_ciudades = atoi(argv[1]);
	Ciudad * ciudades = new Ciudad[num_ciudades+1];

	if (argc == 2) GenerarCiudades(ciudades, num_ciudades);
	else GenerarCiudades(ciudades, num_ciudades, argv[2]);
	// ImprimeCiudades("Mapa de las ciudades: ", ciudades, num_ciudades);


	high_resolution_clock::time_point tantes, tdespues;
  duration<double> transcurrido;
  tantes = high_resolution_clock::now();

	ciudades = RecorreCiudadesA(ciudades, num_ciudades);		// Primer metodo greedy
	//RecorreCiudades(ciudades, num_ciudades);  						// Segundo metodo no greedy

	tdespues = high_resolution_clock::now();
  transcurrido = duration_cast<duration<double>>(tdespues - tantes);
  cout << num_ciudades << " " << transcurrido.count() << endl;

	/*------------------------------------------------------------------------------------
	// DESCOMENTAR ESTO PARA HACER UNA COMPARACIÓN ENTRE LAS DISTANCIAS RECORRIDAS POR AMBOS ALGORITMOS
	// Para su uso ejecutar: ./tsp_v3 <num_ciudades> <nombre_fichero> < <verificacion>

	// ImprimeCiudades("", ciudades, num_ciudades);													// Descomentar para poder ver el camino propio
	double dis_total = DistanciaCamino(ciudades, num_ciudades);							// Calcular distancia total del camino
	cout << "\n\nEl camino total inventado es: " << dis_total << endl;

	int size = num_ciudades;
	int * v = new int[size];
	for (int i = 0; i < size; i++){
		cin >> v[i]; v[i]--;
	}

	double suma = 0;
	for (int i = 0; i < size; i++){
		suma += Distancia(ciudades[v[i]], ciudades[v[i+1]]);							  // Calcular distancia total del camino optimo

		// Para mostrar el camino optimo con coordenadas x e y y poder mostrar el mapa con gnuplot
		// para su uso: ./tsp_v3 <num_ciudades> <nombre_fichero> < <verificacion> > <salida>
		//cout << ciudades[v[i]].id << " " << ciudades[v[i]].x << " " << ciudades[v[i]].y << endl;
	}

	if (suma - dis_total > 0) cout << "Mi algoritmo es mejor" << endl;
	else 											cout << "El algoritmo optimo es mejor" << endl;
	cout << "Mio: " << dis_total << " Tour: " << suma << " Aproximacion: " << (suma/dis_total*100) << endl;
	*/

	return 0;
}

/******************************************************************************/
// Devuelve la distancia euclidiana entre una ciudad y otra
double Distancia(const Ciudad & origen, const Ciudad & destino){
	double dx = destino.x - origen.x;			// Distancia en el eje x
	double dy = destino.y - origen.y;			// Distancia en el eje y
	return sqrt(dx*dx + dy*dy);						// Raiz de la suma de las diferencias al cuadrado
}


/******************************************************************************/
// Elimina la ciudad que se encuentre en la posicion indicada
void EliminarCiudad(const int pos, Ciudad * ciudades, int & size){
	--size;
	for (int i = pos; i < size; ++i) ciudades[i] = ciudades[i+1];
}

bool operator == (const Ciudad & una, const Ciudad & otra){
	return ((una.id == otra.id) && (una.x == otra.x) && (una.y == otra.y));
}

bool operator != (const Ciudad & una, const Ciudad & otra){
	return !(una == otra);
}


bool Esta(const Ciudad & ciudad, const Ciudad * ciudades, const int & tam){
	for (int i = 0; i < tam; ++i) if (ciudades[i] == ciudad) return true;
	return false;
}

/******************************************************************************/
// Busca la ciudad mas lejana desde la ciudad origen
// Si no la encuentra devuelve -1, en otro caso devuelve otra posicion
int BuscaMaxDistancia(const Ciudad & origen, const Ciudad * ciudades, const int size){
	double dist, max = -1;
	int pos_ciu = -1;

	for (int i = 0; i < size; ++i){
		if ((dist=Distancia(origen, ciudades[i])) > max) {max = dist; pos_ciu = i;}
	}

	return pos_ciu;
}


/******************************************************************************/
// Variación del algoritmo que sigue la heuristica del vecino más cercano
// inspirandose del algoritmo A* (estrella), usado en IA para busqueda de camino
Ciudad * RecorreCiudadesA(Ciudad * ciudades, int & num_ciudades){
	Ciudad * res = new Ciudad[num_ciudades+1];
	Ciudad actual = res[num_ciudades] = res[0] = ciudades[0];

	EliminarCiudad(0, ciudades, num_ciudades);
	int pos_max, pos_min, size = num_ciudades;
	double dist, min;

	pos_max = BuscaMaxDistancia(actual, ciudades, size); // ciudad mas lejana a la actual

	for (int i = 1; i < num_ciudades; ++i){
		min = MAX_BUSQUEDA;

		for (int j = 0; j < size; ++j){
			dist = Distancia(actual, ciudades[j]);
			if ((dist+Distancia(ciudades[j], ciudades[pos_max])) < min
				&& (ciudades[j] != ciudades[pos_max])){
				pos_min = j;
				min = dist;
			}
		}

		actual = res[i] = ciudades[pos_min];
		EliminarCiudad(pos_min, ciudades, size);
	}

	res[num_ciudades] = ciudades[pos_max];
	num_ciudades+=2;
	return res;
}

/******************************************************************************/
// Devuelve la secuencia de ciudades que se obtiene al recorrer todas
// las ciudades pasadas desde la primera hasta la ultima volviendo a la primera
void RecorreCiudades(Ciudad * ciudades, int & num_ciudades){
	Ciudad origen = ciudades[0];									// La ciudad origen será la primera

	for  (int i = 0; i < num_ciudades; ++i)
		ciudades[i].distancia = Distancia(origen, ciudades[i]);

	OrdenaCiudades(ciudades, num_ciudades);
	ciudades[num_ciudades] = origen;
	num_ciudades++;
}

/******************************************************************************/
/*// En caada iteracion busca la ciudad mas cercana a la actual y se desplaza a ella
// pasando la cercana a ser la actual, se repite el proceso hasta que se acaben las ciudades
void RecorreCiudadesSimple(Ciudad * ciudades, int & num_ciudades){
	Ciudad * res = new Ciudad[num_ciudades+1];
	Ciudad actual = res[0] = ciudades[0];
	res[num_ciudades] = res[0];

	double dist_actual, min = Distancia(actual, ciudades[1]);
	int pos_ciu = 0, tam = num_ciudades;

	for (int i = 0; i < num_ciudades; ++i){	  // Recorrer todas las ciudades excepto la ciudad origen

		min = 900000000000000;	// Inicializar el minimo
		for (int j = 1; j < tam; ++j){					// Buscar el minimo de todas las ciudades

			dist_actual = Distancia(actual, ciudades[j]);
			if (dist_actual < min){
				min = dist_actual;
				pos_ciu = j;
			}
		}

		cout << "La posicion: " << pos_ciu << endl;
		cout << "El tamanio : " << tam << endl;

		cout << endl << endl;

		actual = res[i] = ciudades[pos_ciu];
		--tam;
		for (int k = pos_ciu; k < tam; ++k) ciudades[k] = ciudades[k+1];
		ImprimeCiudades("Iterando", ciudades, tam);


	}

	res[num_ciudades] = res[0];
	delete [] ciudades;
	ciudades = res;
	++num_ciudades;
}
*/
/******************************************************************************/
// Llama al algoritmo de ordenacion con los parametros adecuados
void OrdenaCiudades(Ciudad * ciudades, const int num_ciudades){
	OrdenaCiudades(ciudades, 0, num_ciudades);
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

/***************************************************************************/
// Ordena por mergesort el vector de ciudades pasado
// el criterio de orden son las distancias
void OrdenaCiudades(Ciudad * v, int inicio, int final){
  int num_elem = final - inicio;                      			// calcular numero total de elementos
  if (num_elem <= 2){                                 			// Si el vector tiene 2 o menos elementos
    if (num_elem == 2 &&
			v[inicio].distancia > v[final-1].distancia){     			// si tiene 2 elementos
      Ciudad intercambia = v[inicio];                   	  // intercambiar
      v[inicio] = v[final-1];
      v[final-1] = intercambia;
    }
  }
  else {                                              			// Si tiene mas de 2 elementos aplicar recursividad
    int medio = (final+inicio)/2;                     			// Calcular el punto medio del vector
    OrdenaCiudades(v, inicio, medio);                       // Aplicar sobre la parte izda
    OrdenaCiudades(v, medio, final);                        // Aplicar sobre la parte dcha

    int izda = inicio, dcha = medio;                  			// Apuntar a primer elemento de cada vector
    Ciudad * res = new Ciudad[num_elem];                    // Reservar espacio para vector auxialiar
    int anterior, i = 0;                              			// i es el indice que recorrera el vector

    while (izda < medio && dcha < final){             			// Recorrer hasta que algun vector acabe
      if (v[izda].distancia < v[dcha].distancia){						// Si izda es menor que dcha escribir izda
				res[i] = v[izda++];
			}
      else {																								// En otro caso escribir dcha
				res[i] = v[dcha++];
			}
      ++i;                                            			// Avanzar posicion de escritura
    }

    while (izda < medio) res[i++] = v[izda++];        			// Escribir posibles elementos de izda
    while (dcha < final) res[i++] = v[dcha++];        			// Escribir posibles elementos de dcha

    memcpy(v+inicio, res, num_elem*sizeof(Ciudad));      		// Copiar los datos ordenados al vector original
    delete [] res;                                    			// Liberar memoria reservada
  }
}


/******************************************************************************/
// Generar aleatoriamente las ciudades reprsentadas por sus coordenadas
void GenerarCiudades(Ciudad * ciudades, const int DIM){
	const int MIN = 0, MAX = 1000;
	for (int i = 0; i < DIM; ++i){
		ciudades[i].id = i;											// Asignar id de la ciudad
		ciudades[i].x  = aleatorio(MIN, MAX);		// Asignar eje x aleatorio
		ciudades[i].y  = aleatorio(MIN, MAX);		// Asignar eje y aleatorio
	}
}

/******************************************************************************/
// Genera un vector de ciudades con el tamaño indicado
// El formato del fichero esperado es 3 numeros por linea, donde
// el primero es el id de la ciudad y los otros sus coordenadas x e y
void GenerarCiudades(Ciudad * ciudades, const int DIM, const char * nombre){
	ifstream fi(nombre);
	if (!fi){Error("No se pudo abrir el fichero indicado");}

	double id, x, y;
	int i = 0;

	while (!fi.eof() && i < DIM){
		fi >> id >> x >> y;
		ciudades[i].id = id; ciudades[i].x = x; ciudades[i].y = y;
		++i;
	}

	fi.close();
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
void ImprimeCiudades(const char * msg, const Ciudad * ciudades, const int num_ciudades){
	cout << msg;
	for (int i = 0; i < num_ciudades; ++i)
		cout << ciudades[i].id << " " << ciudades[i].x << " " << ciudades[i].y << endl;
}

/******************************************************************************/
// Función para notificar error y abortar el programa
void Error(const char * msg){
	cerr << "ERROR: " << msg << endl;
	cerr << "USO: ./<nombre programa> <numero de ciudades> [nombre fichero]" << endl;
	exit(EXIT_FAILURE);
}
