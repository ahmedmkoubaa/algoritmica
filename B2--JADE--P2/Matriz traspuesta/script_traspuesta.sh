#!/bin/bash

#-----------------------------------------------------#
# Pon los tamaños que mejor te funcionen
INICIO=2 				# Tamanio inicial --- pon el que consideres oportuno
FINAL=32768 		# Tamanio final
INCRE=2   			# Tamanio de incremento

#-----------------------------------------------------#
#g++ -o traspuesta_fb traspuesta_fb.cpp -std=gnu++0x

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN traspuesta_fb
rm res/traspuesta_fb.dat
INICIO=2

for ((tam=$INICIO; tam<=$FINAL; tam*=$INCRE)) do
	./traspuesta_fb $tam >> res/traspuesta_fb.dat
done

#-----------------------------------------------------#
#g++ -o traspuesta_dv traspuesta_dv.cpp -std=gnu++0x

#--------------------------------------------------------------------#
# EJECUCIÓN ALGORITMO DE ORDENACIÓN traspuesta_dv
rm res/traspuesta_dv.dat
INICIO=2

for ((tam=$INICIO; tam<=$FINAL; tam+=$INCRE)) do
	./traspuesta_dv $tam >> res/traspuesta_dv.dat
done
