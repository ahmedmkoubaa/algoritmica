// Compilacion:
// g++ -std=gnu++0x tsp.cpp -o tsp  <-- Sin optimizacion
// g++ -O3 -std=gnu++0x tsp.cpp -o tsp-O3  <-- Con optimizacion


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <cstring>
#include <ctime>
#include <chrono>
#include <stdlib.h>

using namespace std::chrono;

high_resolution_clock::time_point tantes, tdespues;
duration<double> transcurrido;

using namespace std;

/**
 * @brief Lee de un archivo .tsp y extrae la informacion de las ciudades
 * 
 * @param archivo nombre del archivo a leer
 * @param ciudades objeto de tipo map donde se guarda la informacion de las ciudades
 */
void LeerCiudades(string archivo, map<int, pair<double, double>> & ciudades) {
    ifstream flujo;
    char cadena[70];
    int N;

    flujo.open(archivo);
    if (flujo) {
        
        flujo >> cadena >> cadena;
        
        if (cadena[0] == 'a') {
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
            
            flujo >> cadena >> N;

            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
        }
        
        else {
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);

            flujo >> cadena >> N;
            
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
            flujo.getline(cadena, 70);
        }
        
        pair<double, double> punto;
        int num_punto;
        
        for (int i=0; i<N; i++) {
            flujo >> num_punto >> punto.first >> punto.second;
            ciudades[num_punto] = punto;
        }
        
        flujo.close();
    }
}

/**
 * @brief Almacena en el mapa ciudades con coordenadas aleatorias
 * 
 * @param ciudades mapa donde se almacenan las ciudades
 * @param numero_ciudades dimension del mapa de ciudades
 */
void AlmacenarCiudadesAleatorias(map<int, pair<double, double>> & ciudades, int numero_ciudades) {
    pair<double, double> punto;
    srand(time(NULL));
    
    for (int i=0; i<numero_ciudades; i++) {
        punto.first = (double) (rand() % 100);
        punto.second = (double) (rand() % 100);
        ciudades[i] = punto;
    }
}

/**
 * @brief Calcula la distacia entre dos puntos
 * 
 * @param p1 punto de la primera ciudad
 * @param p2 punto de la segunda ciudad
 * @return double 
 */
double Distancia(const pair<double, double> & p1, const pair<double, double> & p2) {
    return sqrt(pow(p2.first-p1.first,2) + pow(p2.second-p1.second,2));
}

/**
 * @brief Calcula las distancias que hay entre ciudades y las almacena en una matriz
 * 
 * @param m matriz donde se almacenan los resultados
 * @param ciudades objeto de tipo map que contiene la informacion de las ciudades
 */
void CalculaDistanciaCiudades(double **m, const map<int, pair<double, double>> & ciudades) {
    for (int i=0; i<ciudades.size(); i++) {
        for (int j=0; j<ciudades.size(); j++) {
            m[i][j] = Distancia(ciudades.at(i), ciudades.at(j));
        }
    }
}

/**
 * @brief Obtiene el indice de las ciudades
 * 
 * @param n numero de ciudades
 * @return vector<int> 
 */
vector<int> ObtenerCiudades(int n) {
    vector<int> salida;
    for (int i=1; i<n+1; i++) {
        salida.push_back(i);
    }
    return salida;
}

/**
 * @brief Indica si el indice de una ciudad esta en un vector
 * 
 * @param v vector donde se busca el indice de la ciudad
 * @param ciudad indice de la ciudad
 * @return true si la ciudad se encuentra en el vector
 * @return false si la ciudad no se encuentra en el vector
 */
bool EstaCiudad(const vector<int> & v, int ciudad) {
    bool enc = false;
    for (int i=1; i<v.size()+1 && !enc; i++) {
        if (v[i] == ciudad) {
            enc = true;
        }
    }
    return enc;
}

int buscar(vector<int> v, int valor) {
    bool enc = false;
    int salida;

    for(int i=0; i<v.size() && !enc; i++) {
        if (v[i] == valor) {
            salida = i;
            enc = true;
        }
    }
    return salida;
}

/**
 * @brief Funcion de factibilidad
 * 
 * @param distancia_total distancia solucion actual
 * @param coste valor de coste que hay acumulado de lo que se ha recorrido
 * @param visitadas vector de booleanos que indica si una ciudad (indice) ha sido visitada (true) o no (false)
 * @param distancias matriz que contiene las distancias entre cada dos ciudades
 * @param num_ciudades numero de ciudades
 * @return true si es factible continuar analizando la rama del arbol
 * @return false si no es factible continuar analizando la rama del arbol
 */
bool EsFactible(double distancia_total, double coste, vector<bool> visitadas, double **distancias, int num_ciudades) {
    double estimador = coste;
    double minimo_coste;
    visitadas[0] = false;

    for (int i=0; i<num_ciudades; i++) {
        if (!visitadas[i]) {
            minimo_coste = 1209334;
            // Calculamos el coste minimo de 'salir' de cada una de las ciudades que quedan por recorrer
            for (int j=0; j<num_ciudades; j++) {
                if (!visitadas[j] && distancias[i][j] != 0 && minimo_coste > distancias[i][j]) {
                    minimo_coste = distancias[i][j];
                }
            }
            estimador += minimo_coste;
        }
    }

    return estimador < distancia_total;
}

/**
 * @brief Algoritmo Viajante de Comercio con Backtrancking
 * 
 * @param distancias matriz que contiene las distancias entre cada dos ciudades
 * @param visitadas vector de booleanos que indica si una ciudad (indice) ha sido visitada (true) o no (false)
 * @param posicion_actual posicion del grafo en el que se encuentra el algoritmo (inicialmente posicion 0)
 * @param num_ciudades numero de ciudades
 * @param contador contador de las ciudades recorridas (niveles del grafo)
 * @param coste coste acumulado del recorrido 
 * @param distancia_total distancia total tras haber recorrido todas las ciudades y regresar a la de origen
 * @param recorrido_parcial vector que almacena el recorrido de las ciudades en el calculo (solucion parcial) 
 * @param recorrido_minimo vector que almacena el recorrido minimo de las ciudades (solucion)
 */
void tsp(double **distancias, vector<bool>& visitadas, int posicion_actual, int num_ciudades, int contador, double& coste, double& distancia_total, vector<int>& recorrido_parcial, vector<int>& recorrido_minimo) {
    // Colocamos la ciudad origen (ciudad de indice 0) en el recorrido y la marcamos como visitada
    if (!visitadas[0]) {
        visitadas[0] = true;
        recorrido_parcial.push_back(0);
    }
    
    // Si hemos recorrido todo el grafo y nos encontramos en el ultimo nodo 
    // y existe una distancia entre este y el nodo de origen,
    // comparamos la distancia total con la que ya tenemos y nos quedamos con la minima
    if (contador == num_ciudades && distancias[posicion_actual][0] != 0) {
        if ( distancia_total > (coste + distancias[posicion_actual][0]) ) {
            distancia_total = coste + distancias[posicion_actual][0];
            recorrido_minimo = recorrido_parcial;

            // Añadimos la ciudad de origen como ultima del recorrido (regreso a ciudad de origen: cierre del Ciclo Hamiltoniano)
            recorrido_minimo.push_back(0);
        }
        return; 
    }
    
    if (EsFactible(distancia_total, coste, visitadas, distancias, num_ciudades)) {
        // Recorremos el grafo desde la posicion actual llamando a la misma funcion (recursividad)
        for (int i = 0; i < num_ciudades; i++) {
            
            // Si la ciudad no ha sido visitada, no es la misma que la actual y es factible continuar calculando esta rama del nodo
            if (!visitadas[i] && distancias[posicion_actual][i] != 0) { 
                
                // Marcamos ciudad i como "visitada"
                visitadas[i] = true;
                recorrido_parcial.push_back(i);
                coste += distancias[posicion_actual][i];

                tsp(distancias, visitadas, i, num_ciudades, contador + 1, coste, distancia_total, recorrido_parcial, recorrido_minimo); 
                
                // Marcamos ciudad i como "no visitada"
                visitadas[i] = false;
                recorrido_parcial.pop_back();
                coste -= distancias[posicion_actual][i];
            }
        }
    }
    

}


int main (int argc, char** argv) {

    vector<int> recorrido, ciudades;
    map<int, pair<double, double>> mapa_ciudades;
    double distancia_total = 123421542;

    if (argc != 2) {
        cout << "Error: debe indicar el archivo de datos de entrada" << endl;
        exit(EXIT_FAILURE);
    }

    // Leemos los datos de entrada del archivo pasado como argumento
    LeerCiudades(argv[1], mapa_ciudades);
    
    // int num = atoi(argv[1]);
    // AlmacenarCiudadesAleatorias(mapa_ciudades, num);    // Para medir eficiencia del algoritmo

    // Reservamos memoria dinamica para la matriz que almacena las distancias entre ciudades
    double** distancias = new double*[mapa_ciudades.size()+1];
	for(int i=0; i<mapa_ciudades.size()+1; ++i) distancias[i] = new double[mapa_ciudades.size()+1];

    // Calculamos las distancias entre ciudades y las almacenamos en la matriz
    CalculaDistanciaCiudades(distancias, mapa_ciudades);

    // Obtenemos los indices de las ciudades
    // ciudades = ObtenerCiudades(mapa_ciudades.size());
    
    vector<int> recorrido_parcial;
    
    // Vector visitadas
    vector<bool> visitadas;

    for (int i=0; i<mapa_ciudades.size(); i++) {
        visitadas.push_back(false);
    }
   
    
    
    /********* Ejecucion del algortimo del Viajante de Comercio *********/


    // cout <<"matriz distancias: "<<endl;
    // for (int i=0; i< mapa_ciudades.size(); i++) {
    //     for (int j=0; j< mapa_ciudades.size(); j++) {
    //         cout << distancias[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // SALIDA PARA RESULTADO DEL ALGORITMO
    // Resultado del algoritmo: Recorrido de las ciudades escogido
    double coste = 0;
    
    tsp(distancias, visitadas, 0, mapa_ciudades.size(), 1, coste, distancia_total, recorrido_parcial, recorrido);
   
    cout <<"Numero ciudades: "<< mapa_ciudades.size() << endl;
    cout << "El recorrido (" << recorrido.size() << ") tomado es: " << endl;
    for (int i=0; i<recorrido.size(); i++) {
        cout << recorrido[i] << " " << mapa_ciudades[recorrido[i]].first << " " << mapa_ciudades[recorrido[i]].second << endl;
    }

    cout << endl << "La distancia recorrida es: " << distancia_total << endl;
    

    // // CALCULO PARA MEDIR LA EFICIENCIA DEL ALGORITMO (Descomentar bloque)
    // tantes = high_resolution_clock::now();
    // tsp(distancias, visitadas, 0, mapa_ciudades.size(), 1, coste, distancia_total, recorrido_parcial, recorrido);;
    // tdespues = high_resolution_clock::now();

    // // SALIDA PARA MEDIR LA EFICIENCIA DEL ALGORITMO
    // transcurrido = duration_cast<duration<double>>(tdespues - tantes);
    // cout << argv[1] << " " << transcurrido.count() << endl;


    for(int i=0; i<mapa_ciudades.size()+1; ++i) delete[] distancias[i];
    delete[] distancias;

    

}