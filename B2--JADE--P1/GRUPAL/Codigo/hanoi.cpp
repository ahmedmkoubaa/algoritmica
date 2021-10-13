/**
   @file Resoluci�n del problema de las Torres de Hanoi
*/


#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>

using namespace std;
using namespace std::chrono;

/**
   @brief Resuelve el problema de las Torres de Hanoi
   @param M: n�mero de discos. M > 1.
   @param i: n�mero de columna en que est�n los discos.
             i es un valor de {1, 2, 3}. i != j.
   @param j: n�mero de columna a que se llevan los discos.
             j es un valor de {1, 2, 3}. j != i.

   Esta funci�n imprime en la salida est�ndar la secuencia de
   movimientos necesarios para desplazar los M discos de la
   columna i a la j, observando la restricci�n de que ning�n
   disco se puede situar sobre otro de tama�o menor. Utiliza
   una �nica columna auxiliar.
*/
void hanoi (int M, int i, int j);




void hanoi (int M, int i, int j)
{
  if (M > 0)
    {
      hanoi(M-1, i, 6-i-j);
      hanoi (M-1, 6-i-j, j);
  }
}

/*usamos high_resolution_clock debemos compilar asi:
   g++ -o quicksort quicksort.cpp -std=gnu++0x
*/
int main(int argc, char ** argv)
{
   if (argc < 2){
      cerr << "Error: numero de parametros incorrectos" << endl;
      cerr << "USO: " << argv[0] << " <numero elementos>" << endl;
      exit(EXIT_FAILURE);
   }

   high_resolution_clock::time_point tantes, tdespues;
   duration<double> transcurrido;

   int num_discos = atoi(argv[1]);

   tantes = high_resolution_clock::now();
   hanoi(num_discos, 1, 2);
   tdespues = high_resolution_clock::now();
   transcurrido = duration_cast<duration<double>>(tdespues - tantes);
   cout << num_discos << " " << transcurrido.count() << endl;

   return 0;
}
