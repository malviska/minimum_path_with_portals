#include "heap.h"
#include "graph.h"
#include<stdio.h>
#include<stdlib.h>


void dijkstra(Graph * graph, int * distances, int ** coord){
  
}

int main(){
  int n, m, k;
  int s, q;
  scanf("%d %d %d", &n, &m, &k);
  int** coord = (int **) malloc(n*sizeof(int *));
  for(int i = 0; i<n; i++){
    coord[i] = (int *) malloc(2*sizeof(int));
  }
  Graph * graph = createGraph(n);

  for(int i = 0; i<n; i++){
    int x, y;
    scanf("%d %d", &x, &y);
    coord[i][0] = x;


    coord[i][1] = y;
  }
  for(int i = 0; i<m; i++){
    int v1, v2, portal = 0;
    scanf("%d %d", &v1, &v2);
    addEdge(graph, v1, v2, portal);
  }
  for(int i = 0; i<k; i++){
    int v1, v2, portal = 1;
    scanf("%d %d", &v1, &v2);
    addEdge(graph, v1, v2, portal);
  }
  scanf("%d %d", &s, &q);

  printGraph(graph);

  //deallocate memory
  destroyGraph(graph);
  for(int i = 0; i<n; i++){
    free(coord[i]);
  }
  free(coord);

  return 0;
  
}