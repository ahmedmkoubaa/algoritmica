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

  srand(time(0));
  //para generar un vector que pueda tener elementos repetidos
  for (int j=0; j<n; j++) {
    double u=uniforme();
    int k=(int)(n*u);
    T[j]=k;
  }


  tantes = high_resolution_clock::now();
  EliminaRepetidos(T, n);
  tdespues = high_resolution_clock::now();
  transcurrido = duration_cast<duration<double>>(tdespues - tantes);
  cout << argv[1] << " " << transcurrido.count() << endl;

  delete [] T;
  return (0);
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


// No se puede hacer una version DyV del algoritmo de eliminacion de
// repetidos pues las divisiones hacen que los elementos del vector se excluyan
// y no se puede intentar resolver un subproblema comunicandolo con otro
// pues eso es equivalente a resolver el problema general en partes y mal
// por eso apostamos por ir dividiendo el vector principal y ordenandolo
// para posteriormente recorrerlo y eliminar los repetidos.
void EliminaRepetidosDV(int * v, int & n){
  Ordena(v, 0, n);
  EliminaRepetidosOrdenado(v,n);
}

/***************************************************************************/
// Recorre el vector y elimina aquellos elementos repetidos
void EliminaRepetidosOrdenado(int * v, int & n){
  int esc, lec;     //Para leer y escribir
  lec = esc = 0;

  while (lec < n){
    ++lec;
    if (v[esc] != v[lec]){esc++; v[esc] = v[lec];}
  }

  n = esc;
}

/***************************************************************************/
// Elimina los elementos repetidos del vector que empieza en
// inicio y acaba en final
void Ordena(int * v, int inicio, int final){
  int num_elem = final - inicio;        // calcular numero total de elementos
  if (num_elem <= 2){                   // Si el vector tiene 2 o menos elementos
    if (num_elem == 2 && v[inicio] > v[final-1]){                 // si tiene 2 elementos
      int intercambia = v[inicio];      // intercambiar
      v[inicio] = v[final-1];
      v[final-1] = intercambia;
    }
  }
  else {                                              // Si tiene mas de 2 elementos aplicar recursividad
    int medio = (final+inicio)/2;                     // Calcular el punto medio del vector
    Ordena(v, inicio, medio);             // Aplicar sobre la parte izda
    Ordena(v, medio, final);              // Aplicar sobre la parte dcha

    int izda = inicio, dcha = medio;                  // Apuntar a primer elemento de cada vector
    int * res = new int[num_elem];
    int anterior, i = 0;                              // i es el indice que recorrera el vector

    while (izda < medio && dcha < final){
      if (v[izda] < v[dcha]) res[i] = v[izda++];
      else                   res[i] = v[dcha++];
      ++i;
    }

    while (izda < medio) res[i++] = v[izda++];
    while (dcha < final) res[i++] = v[dcha++];

    memcpy(v+inicio, res, num_elem*sizeof(int));      // Copiar los datos ordenados
    delete [] res;                                    // Liberar memoria reservada
  }
}





















// algo
