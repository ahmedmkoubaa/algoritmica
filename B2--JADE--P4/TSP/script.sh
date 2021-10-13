#!/bin/bash

INI=3
FINAL=22
INCRE=1


###############################################################
# Preparar directorios y ficheros resultado
mkdir res;

rm res/tsp_efi.dat
rm res/tsp_efi_o3.dat

rm res/tsp_inefi.dat
rm res/tsp_inefi_o3.dat

###############################################################
# compilar y ejecutar para todos los tamaños, resultado en fichero
g++ -o tsp_efi tsp_efi.cpp -std=c++11;
for ((tam=$INI; tam<=$FINAL; tam+=$INCRE)) do
	./tsp_efi $tam ulysses22.dat >> res/tsp_efi.dat
done

###############################################################
# compilar y ejecutar para todos los tamaños, resultado en fichero
g++ -o tsp_efi tsp_efi.cpp -std=c++11 -O3;
for ((tam=$INI; tam<=$FINAL; tam+=$INCRE)) do
	./tsp_efi $tam ulysses22.dat >> res/tsp_efi_o3.dat;
done

###############################################################
# compilar y ejecutar para todos los tamaños, resultado en fichero
g++ -o tsp_inefi tsp_inefi.cpp -std=c++11;
for ((tam=$INI; tam<$FINAL; tam+=$INCRE)) do
	./tsp_inefi $tam ulysses22.dat >> res/tsp_inefi.dat
done

###############################################################
# compilar y ejecutar para todos los tamaños, resultado en fichero
g++ -o tsp_inefi tsp_inefi.cpp -std=c++11 -O3;
for ((tam=$INI; tam<$FINAL; tam+=$INCRE)) do
	./tsp_inefi $tam ulysses22.dat >> res/tsp_inefi_o3.dat;
done
