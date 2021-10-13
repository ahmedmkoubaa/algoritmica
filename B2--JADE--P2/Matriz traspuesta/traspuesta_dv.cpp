#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <cstring>

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
// Lleva a cabo la trasposicion de la matriz pasada mediante un algoritmo DV
void TrasponerDV(int ** m, const int dim);

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
  //ImprimirMatriz("Matriz antes:\n", matriz, DIM);

  high_resolution_clock::time_point tantes, tdespues;
  duration<double> transcurrido;
  tantes = high_resolution_clock::now();

  TrasponerDV(matriz, DIM);

  tdespues = high_resolution_clock::now();
  transcurrido = duration_cast<duration<double>>(tdespues - tantes);
  cout << DIM << " " << transcurrido.count() << endl;

  //ImprimirMatriz("\nDespués trasposicion: \n", matriz, DIM);
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
// Lleva a cabo la trasposicion de la matriz pasada mediante un algoritmo DV
// fi es la fila inicial, ff la final, ci la columna inicial y cf la columna final
void TrasponerDV(int ** m, const int fi, const int ff, const int ci, const int cf){
  if (cf-ci > 2){   // Si la dimension de la matriz es mayor que dos
    // Guardar filas y columnas de inicio y final de cada sumbatriz
    int fi1 = fi, ff1 = (fi+ff)/2, ci1 = ci,  cf1 = (cf+ci)/2;  // sb1) Comienza en filas y columnas de la original y acaba en el centro
    int fi2 = fi, ff2 = ff1,       ci2 = cf1, cf2 = cf;         // sb2) Coincice con sb1 en filas y comienza en el centro de las columnas
    int fi3 = ff1, ff3 = ff,       ci3 = ci,  cf3 = cf1;        // sb3) Comienza en el fin de sb1 en filas, coincide en columnas con sb1
    int fi4 = ff1, ff4 = ff,       ci4 = cf1, cf4 = cf;         // sb$) Coincide con sb3 en filas y con sb2 en columnas

    TrasponerDV(m, fi1, ff1, ci1, cf1);     // Submatriz 1
    TrasponerDV(m, fi2, ff2, ci2, cf2);     // Submatriz 2
    TrasponerDV(m, fi3, ff3, ci3, cf3);     // Submatriz 3
    TrasponerDV(m, fi4, ff4, ci4, cf4);     // Submatriz 4

    const int tam = (cf-ci)/2;              // Numero de elementos a mover en una fila
    const size_t tamB = tam*sizeof(int);    // Tamaño de la fila a mover en Bytes
    int * fila_actual = new int[tam];

    // Intercambiar submatrices 2 y 3
    for (int i = 0; i < tam; ++i){                        // Recorrer todas las filas de sb2
      memcpy(fila_actual,      m[i + fi2] + ci2, tamB);   // Mover todos los elementos de una fila de sb2 a aux
      memcpy(m[i + fi2] + ci2, m[fi3 + i] + ci3, tamB);   // Mover todos los elementos de una fila de sb3 a sb2
      memcpy(m[fi3 + i] + ci3, fila_actual,      tamB);   // Mover todos los elementos de una fila de aux a sb3
    }

    delete [] fila_actual;                                // Liberar espacio reservado
  }
  else {    // Hemos llegado al caso base matriz de dimension 2
    int intercambia = m[fi][cf-1];    // Pasar elemento diagonal a aux
    m[fi][cf-1]     = m[ff-1][ci];    // Mover otro elemento al anterior
    m[ff-1][ci]     = intercambia;    // Poner el primer elemento en donde estaba el otro
  }
}


/*****************************************************************************/
// Lleva a cabo la trasposicion de la matriz pasada mediante un algoritmo DV
void TrasponerDV(int ** m, const int dim){
  TrasponerDV(m, 0, dim, 0, dim);
}

/*****************************************************************************/
// Modifica la matriz pasada convirtiendola en la traspuesta
void ERROR(const char * msg){
  cerr << "Error: " << msg << endl;
  cerr << "USO: ./<programa> <dimensión matriz>" << endl;
  exit(EXIT_FAILURE);
}













































//hola mundo
