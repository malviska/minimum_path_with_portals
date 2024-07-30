#!/bin/bash

numVertices=(10000)
numEdges=(1000 2500 5000 7500 10000 25000 50000 75000 100000 250000 500000)
ratio=0.05

for val in "${numVertices[@]}"
do
  echo "For val=$val"
  ./bin/gen.out "$val" "42892" "2258" >> results4.csv
  ./bin/tp2.out < ./tests/graph.txt >> results4.csv
  ./bin/matrix.out < ./tests/graph.txt >> results4.csv
  echo "Done with $val"
done
