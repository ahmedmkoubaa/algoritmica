#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <cstring>

using namespace std;
using namespace std::chrono;

//generador de ejemplos para el problema de eliminar elementos repetidos. Simplemente, para rellenar el vector de tama�o n genera n enteros aleatorios entre 0 y n-1

double uniforme() //Genera un n�mero uniformemente distribuido en el
                  //intervalo [0,1) a partir de uno de los generadores
                  //disponibles en C.
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

/***************************************************************************/
// Elimina los elementos que se encuentren repetidos en el vector
void EliminaRepetidos(int * v, int & n);

/***************************************************************************/
// Elimina los elementos que se encuentren repetidos en el vector
void EliminaRepetidosDV(int * v, int & n);

/***************************************************************************/
// Ordena por divide y venceras el vector pasado
void EliminaRepetidosDV(int * v, int & n);

/***************************************************************************/
// Elimina los elementos repetidos del vector que empieza en
// inicio y acaba en final
void Ordena(int * v, int inicio, int final);

/***************************************************************************/
// Recorre el vector y elimina aquellos elementos repetidos
void EliminaRepetidosOrdenado(int * v, int & n);

/***************************************************************************/
// Muestra los n elementos del vector v
void ImprimeVector(int * v, const int n);

int main(int argc, char * argv[])
{

  if (argc != 2){
    cerr << "Formato " << argv[0] << " <num_elem>" << endl;
    return -1;
  }

  high_resolution_clock::time_point tantes, tdespues;
  duration<double> transcurrido;
  int n = atoi(argv[1]);

  int * T = new int[n];
  assert(T);

  srand(time(0)); //para generar un vector que pueda tener elementos repetidos
  for (int j=0; j<n; j++) {
    double u=uniforme();
    int k=(int)(n*u);
    T[j]=k;
  }

  //ImprimeVector(T, n);
  tantes = high_resolution_clock::now();
  EliminaRepetidosDV(T, n);
  tdespues = high_resolution_clock::now();
  transcurrido = duration_cast<duration<double>>(tdespues - tantes);
  cout << argv[1] << " " << transcurrido.count() << endl;
  // ImprimeVector(T, n);

  delete [] T;
  return (0);
}

/***************************************************************************/
// Muestra los n elementos del vector v
void ImprimeVector(int * v, const int n){
  cout << "El tamaño del vector es: " << n << endl;
  cout << "El vector es: ";
  for (int i = 0; i < n; ++i) cout  << v[i] << " "; cout << endl;
}

/***************************************************************************/
// Elimina los elementos que se encuentren repetidos en el vector
void EliminaRepetidos(int * v, int & n){
  int avanza;
  for (int i = 0; i < n; i += avanza){
    avanza = 1;

    for (int j = i+1; j < n; ++j){
      if (v[i] == v[j]){
        v[i] = v[--n];
        avanza = 0;
      }
    }
  }
}


/***************************************************************************/
// No se puede hacer una version DyV del algoritmo de eliminacion de
// repetidos pues las divisiones hacen que los elementos del vector se excluyan
// y no se puede intentar resolver un subproblema comunicandolo con otro
// pues eso es equivalente a resolver el problema general en partes y mal
// por eso apostamos por ir dividiendo el vector principal y ordenandolo
// para posteriormente recorrerlo y eliminar los repetidos.
void EliminaRepetidosDV(int * v, int & n){
  Ordena(v, 0, n);                  // Ordenar elementos
  EliminaRepetidosOrdenado(v,n);    // Eliminar elemenos repetidos ya ordenados
}

/***************************************************************************/
// Recorre el vector y elimina aquellos elementos repetidos
void EliminaRepetidosOrdenado(int * v, int & n){
  int esc, lec;                     // Para leer y escribir
  lec = esc = 0;                    // Posicion inicial es 0

  while (lec < n){                  // Recorrer todos los elementos
    ++lec;                          // Incrementar indice de lectura
    if (v[esc] != v[lec]){          // Si los elementos actuales son diferentes
      esc++; v[esc] = v[lec];}      // escribirlos, en otro caso pasar al siguiente
  }

  n = esc;                          // Tamaño vector es num elementos escritos
}

/***************************************************************************/
// Elimina los elementos repetidos del vector que empieza en
// inicio y acaba en final
void Ordena(int * v, int inicio, int final){
  int num_elem = final - inicio;                      // calcular numero total de elementos
  if (num_elem <= 2){                                 // Si el vector tiene 2 o menos elementos
    if (num_elem == 2 && v[inicio] > v[final-1]){     // si tiene 2 elementos
      int intercambia = v[inicio];                    // intercambiar
      v[inicio] = v[final-1];
      v[final-1] = intercambia;
    }
  }
  else {                                              // Si tiene mas de 2 elementos aplicar recursividad
    int medio = (final+inicio)/2;                     // Calcular el punto medio del vector
    Ordena(v, inicio, medio);                         // Aplicar sobre la parte izda
    Ordena(v, medio, final);                          // Aplicar sobre la parte dcha

    int izda = inicio, dcha = medio;                  // Apuntar a primer elemento de cada vector
    int * res = new int[num_elem];                    // Reservar espacio para vector auxialiar
    int anterior, i = 0;                              // i es el indice que recorrera el vector

    while (izda < medio && dcha < final){             // Recorrer hasta que algun vector acabe
      if (v[izda] < v[dcha]) res[i] = v[izda++];      // Si izda es menor que dcha escribir izda
      else                   res[i] = v[dcha++];      // En otro caso escribir dcha
      ++i;                                            // Avanzar posicion de escritura
    }

    while (izda < medio) res[i++] = v[izda++];        // Escribir posibles elementos de izda
    while (dcha < final) res[i++] = v[dcha++];        // Escribir posibles elementos de dcha

    memcpy(v+inicio, res, num_elem*sizeof(int));      // Copiar los datos ordenados al vector original
    delete [] res;                                    // Liberar memoria reservada
  }
}





















// algo
