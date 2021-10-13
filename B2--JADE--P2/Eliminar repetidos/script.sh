#!/bin/bash

barras=('|' '/' '-' '\');
for ((i=0; $i<4; i++, i%=4)) do
  clear;
  printf " %s " ${barras[$i]}; date +%H:%M:%S;
  sleep 0.25;
done
