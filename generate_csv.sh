#!/bin/bash

numVertices=(500000 750000 1000000)
params1=(0.3 0.09)
params2=(0.0005 0.00001)
params3=(0.00005 0.00001)

for val in "${numVertices[@]}"
do
  if [ "$val" -lt 1000 ]; then
    ./bin/gen.out "$val" "${params1[0]}" "${params1[1]}" >> results.csv
  elif [ "$val" -lt 100000 ]; then
    ./bin/gen.out "$val" "${params2[0]}" "${params2[1]}" >> results.csv
  else
    ./bin/gen.out "$val" "${params3[0]}" "${params3[1]}" >> results.csv
  fi
  ./bin/tp2.out < ./tests/graph.txt >> results.csv
  #./bin/matrix.out < ./tests/graph.txt >> results.csv
  echo "Done with $val"
done
