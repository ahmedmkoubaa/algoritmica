/*
Programado por Damián Marín Fernández
Ejercicio ITV:
Una estación de ITV consta de m líneas de inspección de vehículos iguales. Hay un total de
n vehículos que necesitan inspección. En función de sus características, cada vehículo
tardara en ser inspeccionado un tiempo ti; i = 1,...., n. Se desea encontrar la manera de
atender a todos los n vehículos y acabar en el menor tiempo posible. Diseñamos e
implementamos un algoritmo vuelta atrás que determine como asignar los vehículos a las
líneas. Utilizando una función de factibilidad y una poda.
*/
//compilar g++ main.cpp -o main -std=gnu++0x

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdlib>
#include <chrono>
using namespace std::chrono;
#include "solucion.h"

void back_recursivo(solucion &s) {
    if(s.size()<s.num_vehiculos()) {
        int lin=0;						//contador para iterar sobre las líneas
        s.anadir_vehiculo();			//añadimos un vehiculo a la linea 0
        while(lin<s.num_lineas()) {
            if(s.correcto()) {
                back_recursivo(s);
                s.borrar_vehiculo(); //elimina el vehiculo temporal creado en back_recursivo
            }
            s.ultimo_vehiculo_cambio();	//esto es para seguir explorando soluciones cambiando la línea
            lin++;		//cambiamos de línea
        }
    }
    else {		//tenemos que comprobar que la solucion sea correcta
        s.solucion_actualizada();	//se actualiza la solucion y la cota
        s.anadir_vehiculo(); //Añade un vehiculo extra
    }
}




int main(int argc, char *argv[]) {
    high_resolution_clock::time_point t_antes, t_despues;
    duration<double> transcurrido;
    if(argc != 3) {
        std::cerr << "Formato " << argv[0] << " <num_vehiculos> <num_lineas" << "\n";
        return -1;
    }
    int n = atoi(argv[1]);
    int l=atoi(argv[2]);
    solucion s(n,l);

    t_antes = high_resolution_clock::now();
    back_recursivo(s);
    //s.mostrar();
    t_despues = high_resolution_clock::now();
    transcurrido = duration_cast<duration<double> >(t_despues - t_antes);
    std::cout <<"Para "<<n<< " vehiculos ha tardado: " << transcurrido.count() << "\n";
    return 0;
}
