#!/bin/bash


for ((i=5; i<=14; i++))
do
	./main $i 3
done

for ((i=5; i<=14; i++))
do
	./main $i 5
done

for ((i=5; i<=14; i++))
do
	./main $i 7
done
