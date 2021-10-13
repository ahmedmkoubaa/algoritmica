#!/bin/bash

INI=500
FINAL=100000
INCRE=7500


g++ -o tsp_a tsp_v3.cpp -O3;
mkdir res;
rm res/tsp_a;

for ((tam=$INI; tam<$FINAL; tam+=$INCRE)) do
	./tsp_a $tam >> res/tsp_a;
done
