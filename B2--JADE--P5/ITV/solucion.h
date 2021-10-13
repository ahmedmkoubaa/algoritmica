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

using namespace std;

class solucion {
private:

	vector<int> solu;	//vector solución, cada posición es un coche y su correspondiente valor la cola
	vector<int> x;
	vector<int> vehiculos;	//cada posicion un coche y su correspondiente tiempo.
	int lineas, tiempo_min;
public:
	solucion (int num_lineas,int num_vehiculos) {
		srand(time(0));
		for (int i=0; i<num_vehiculos; i++) {
			vehiculos.push_back((rand()%50)+1);	//genera un aleatorio entre 1 y 50
		}
		lineas=num_lineas;
		calculo_greedy();	//resultado greedy que usamos como referencia
	}

	int calcular_tiempo (const vector<int> &x2) const { //calculamos el de x2
		vector<int> tiempos(lineas,0);	//vector de tiempos de cada linea
		for (int i=0; i<x2.size();i++) {
			tiempos[x2[i]]+=vehiculos[i];	//en la cola dada por x[i] añadimos el tiempo del vehiculo i
		}
		int a=tiempos[0];
		for (int i=1; i<tiempos.size();i++) {
			if (tiempos[i]>a) a=tiempos[i];
		}
		return a;
	}

	//Calcula una solucion mediante algoritmo greedy como cota
    void calculo_greedy() {
        vector<int> tiempos(lineas,0);
        for(int i=0; i<vehiculos.size(); i++) {
            int lin_min=0; //linea con menor t
            for(int j=1; j<tiempos.size() && tiempos[lin_min]>0; j++)
                if(tiempos[lin_min]>tiempos[j]) lin_min=j;
            tiempos[lin_min]+=vehiculos[i]; //añade el siguiente vehiculo a la linea con menos tiempo
            solu.push_back(lin_min);
            tiempo_min=calcular_tiempo(solu); //calcula el tiempo solucion de greedy
        }
    }

	//añadimos un vehiculo a la solucion parcial x
    void anadir_vehiculo() {
        x.push_back(0);
    }
    int num_vehiculos() {
        return vehiculos.size();
    }
    int size() {
        return x.size();
    }

    int num_lineas() {
        return lineas;
    }
    void ultimo_vehiculo_cambio() {
        x[x.size()-1]++;
    }
    void borrar_vehiculo() {
        x.pop_back();
    }
    void mostrar() {
        cout<<"Vehiculos:"<<vehiculos.size()<<"     lineas:"<<lineas<<endl;
        cout<<"Tiempo Solucion:"<<tiempo_min<<endl;
        for(int i=0; i<vehiculos.size(); i++) {
            cout<<"Vehiculo "<<i<<"("<<vehiculos[i]<<") -- linea "<<solu[i]<<endl;
        }
    }

    void solucion_actualizada() {
        int tim=calcular_tiempo(x);
        if(tim<tiempo_min) {
            solu=x;
            tiempo_min=tim;
        }
    }
    bool correcto() {
        //no puede darse el caso de que no se siga el orden para coger.
        return (calcular_tiempo(x)<tiempo_min);
    }

};
