#!/bin/bash

#-----------------------------------------------------#
# Para los algoritmos de ordenacion cuadraticos
INICIO=100 		# Tamanio inicial
FINAL=100000 	# Tamanio final
INCRE=5000   	# Tamanio de incremento

#-----------------------------------------------------#
g++ -o elimina_rep_fb elimina_rep_fb.cpp

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN elimina_rep_fb
INICIO=100
rm res/elimina_rep_fb.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./elimina_rep_fb $tam >> res/elimina_rep_fb.dat
done

#-----------------------------------------------------#
g++ -o elimina_rep_dv elimina_rep_dv.cpp

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN elimina_rep_dv
INICIO=100
rm res/elimina_rep_dv.dat
for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./elimina_rep_dv $tam >> res/elimina_rep_dv.dat
done
