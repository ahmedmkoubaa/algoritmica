#include <iostream>
using namespace std;

/************************************************************************************/
// Devuelve la subsecuencia mas larga de caracteres que coincidan en
// orden en sus respectivas cadenas sin importar la contiguidad
string MaxSecuencia(const string & s, const string & t){
  string max = "";      // secuencia resultado a devolver
  bool encontrado;      // para buscar caracteres coincidentes
  int inicio = 0;       // por donde empezar a buscar (respetar el orden)

  for (int i = 0; i < s.size(); ++i){                           // recorrer s
    encontrado = false;                                         // aun no encontrado
    for (int j = inicio; j < t.size() && !encontrado; ++j){     // buscar en t
      if (s[i] == t[j]){                                        // si coinciden
        encontrado = true;                                      // actualizar encontrado
        max += s[i];                                            // añadir a la cadena max
        inicio = j+1;                                           // actualizar posicion
      }
    }
  }

  return max;       // devolver la cadena encontrada
}

/************************************************************************************/
int main (int argc, char ** argv){

  //---------------------------------------------------------------------------//
  // Comprobar datos de entrada
  if (argc != 3){
    cerr << "Error: faltan argumentos" << endl;
    cerr << "USO: " << argv[0] << " <cadena1> <cadena2>" << endl;
    exit(EXIT_FAILURE);
  }

  //---------------------------------------------------------------------------//
  // Guardar los datos y operar con ellos
  string s(argv[1]), t(argv[2]);
  cout << s << endl << t << endl << MaxSecuencia(s,t) << endl;
}
