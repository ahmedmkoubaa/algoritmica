#!/bin/bash

INI=3
FINAL=14
INCRE=1


###############################################################
# Preparar directorios y ficheros resultado
mkdir res;

rm res/tsp_bb.dat
rm res/tsp_bb_o3.dat

###############################################################
# compilar y ejecutar para todos los tamaños, resultado en fichero
g++ -o tsp_bb tsp_bb.cpp -std=c++11;
for ((tam=$INI; tam<=$FINAL; tam+=$INCRE)) do
	./tsp_bb $tam ulysses16.dat >> res/tsp_bb.dat
done

###############################################################
# compilar y ejecutar para todos los tamaños, resultado en fichero
g++ -o tsp_bb tsp_bb.cpp -std=c++11 -O3;
for ((tam=$INI; tam<=$FINAL; tam+=$INCRE)) do
	./tsp_bb $tam ulysses16.dat >> res/tsp_bb_o3.dat;
done
