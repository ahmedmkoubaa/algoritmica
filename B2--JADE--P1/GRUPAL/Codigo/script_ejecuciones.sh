#!/bin/bash

#-----------------------------------------------------#
# Para los algoritmos de ordenacion cuadraticos
INICIO=10000 # Tamanio inicial
FINAL=200000 # Tamanio final
INCRE=7600   # Tamanio de incremento
#-----------------------------------------------------#

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN BURBUJA
rm res/burbuja.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./burbuja $tam >> res/burbuja.dat
done
#--------------------------------------------------------------------#

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN INSERCIÓN
rm res/insercion.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./insercion $tam >> res/insercion.dat
done
#--------------------------------------------------------------------#

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN SELECCION
rm res/seleccion.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./seleccion $tam >> res/seleccion.dat
done
#--------------------------------------------------------------------#

#--------------------------------------------------------------------#
# Redefinir el tamaño de pruea para algoritmos mas eficientes
INICIO=10000; FINAL=200000000; INCRE=7999600

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN MERGESORT
rm res/mergesort.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./mergesort $tam >> res/mergesort.dat
done
#--------------------------------------------------------------------#

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN QUICKSORT
rm res/quicksort.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./quicksort $tam >> res/quicksort.dat
done
#--------------------------------------------------------------------#

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN HEAPSORT
rm res/heapsort.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./heapsort $tam >> res/heapsort.dat
done
#--------------------------------------------------------------------#

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE FLOYD
# Redefinir tamaños de prueba para acabar antes del plazo de entrega
INICIO=300; FINAL=2800; INCRE=100;
rm res/floyd.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./floyd $tam >> res/floyd.dat
done
#--------------------------------------------------------------------#

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE HANOI
# Redefinir tamaños de prueba para acabar algun dia
INICIO=20; FINAL=35; INCRE=1;
rm res/hanoi.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./hanoi $tam >> res/hanoi.dat
done
#--------------------------------------------------------------------#
