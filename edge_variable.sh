#!/bin/bash

numEdges=(1000 2500 5000 7500 10000 25000 50000 75000 100000 125000 150000 175000 200000)
ratio=0.0

for val in "${numEdges[@]}"
do
  portals=$(echo "scale=0; ($val * $ratio + 0.5) / 1" | bc)
  path=$((val - portals))
  echo "For val=$val: portals=$portals, path=$path"
  ./bin/gen.out "1001" "$path" "$portals" >> results3.csv
  ./bin/tp2.out < ./tests/graph.txt >> results3.csv
  ./bin/matrix.out < ./tests/graph.txt >> results3.csv
  echo "Done with $val"
done
