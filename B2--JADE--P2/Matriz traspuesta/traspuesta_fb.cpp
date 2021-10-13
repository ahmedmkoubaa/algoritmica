#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>

using namespace std;
using namespace std::chrono;

/******************************************************************************/
// Reservar espacio para matriz de dimensión m
int ** ReservarMatriz(const int dim);

/******************************************************************************/
// Libera la memori reservada previamente para la matriz
void LiberarMatriz(int ** m, const int dim);

/******************************************************************************/
// Asigna a cada elemento de la matriz un indice que hace referencia a su
// posicion en la tabla
void RellenarMatriz(int ** m, const int dim);

/*****************************************************************************/
// Para mostrar la matriz por pantalla
void ImprimirMatriz(const char * msg, int ** m, const int dim);

/*****************************************************************************/
// Modifica la matriz pasada convirtiendola en la traspuesta
void TrasponerMatriz(int ** m, const int dim);

/*****************************************************************************/
// Modifica la matriz pasada convirtiendola en la traspuesta
void ERROR(const char * msg);

int main(int argc, char ** argv){
  if (argc != 2){
    ERROR("número de argumentos incorrecto");
  }

  const int DIM = atoi(argv[1]);

  if ((DIM*DIM)%4){
    ERROR("Matriz debe tener 2^k elementos. Dimension debe ser multiplo de 4");
  }


  int ** matriz = ReservarMatriz(DIM);
  RellenarMatriz(matriz, DIM);
  //ImprimirMatriz("Estado actual de la matriz previa trasposicion\n", matriz, DIM);

  high_resolution_clock::time_point tantes, tdespues;
  duration<double> transcurrido;

  tantes = high_resolution_clock::now();
  TrasponerMatriz(matriz, DIM);
  tdespues = high_resolution_clock::now();
  transcurrido = duration_cast<duration<double>>(tdespues - tantes);
  cout << DIM << " " << transcurrido.count() << endl;

  //ImprimirMatriz("\nEstado posterior de la matriz\n", matriz, DIM);
  LiberarMatriz(matriz, DIM);

  return(EXIT_SUCCESS);
}


/******************************************************************************/
// Reservar espacio para matriz de dimensión m
int ** ReservarMatriz(const int dim){
  int ** m = new int * [dim];
  for (int i = 0; i < dim; ++i) m[i] = new int[dim];
  return m;
}

/******************************************************************************/
// Libera la memori reservada previamente para la matriz
void LiberarMatriz(int ** m, const int dim){
  for (int i = 0; i < dim; i++){
    delete [] m[i];
  }

  delete [] m;
}


/******************************************************************************/
// Asigna a cada elemento de la matriz un indice que hace referencia a su
// posicion en la tabla
void RellenarMatriz(int ** m, const int dim){
  int elemento = 0;
  for (int i = 0; i < dim; i++){
    for (int j = 0; j < dim; j++){
      m[i][j] = ++elemento;
    }
  }
}

/******************************************************************************/
// Para mostrar la matriz por pantalla
void ImprimirMatriz(const char * msg, int ** m, const int dim){
  cout << msg;
  for (int i = 0; i < dim; i++){
    for (int j = 0; j < dim; j++){
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}

/*****************************************************************************/
// Modifica la matriz pasada convirtiendola en la traspuesta
void TrasponerMatriz(int ** m, const int dim){
  for (int i = 0; i < dim; ++i){
    for (int j = i+1; j < dim; ++j){
      int intercambia = m[i][j];
      m[i][j] = m[j][i];
      m[j][i] = intercambia;
    }
  }
}

/*****************************************************************************/
// Modifica la matriz pasada convirtiendola en la traspuesta
void ERROR(const char * msg){
  cerr << "Error: " << msg << endl;
  cerr << "USO: ./<programa> <dimensión matriz>" << endl;
  exit(EXIT_FAILURE);
}













































//hola mundo
