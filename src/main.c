#include "heap.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>



void dijkstra(Graph * graph, double * distances, double ** coord, int maxPortals){
  for (size_t i = 0; i<graph->size; i++){
    distances[i] = DBL_MAX;
  }
  Heap * minHeap = heapNew(graph->size*2);
  heapInsert(minHeap, 0, 0, 0);
  while(heapEmpty(minHeap) == 0){
    Path * path = heapRemove(minHeap);
    if(distances[path->vertex] < path->distance || path->portals > maxPortals){
      continue;
    }
    distances[path->vertex] = path->distance;
    Node * node = graph->adjList[path->vertex];
    while(node != NULL){
      double d = path->distance;
      if(node->portal == 0){
        double x = coord[node->vertex][0];
        double y = coord[node->vertex][1];
        double x_init = coord[path->vertex][0];
        double y_init = coord[path->vertex][1];
        d = sqrt(pow(x-x_init, 2) + pow(y-y_init, 2)) + d;
       }
      int portals = node->portal + path->portals;
      heapInsert(minHeap, node->vertex, d, portals);
      node = node->next;
    }
  }
  heapDestroy(minHeap);
}

int main(){
  int n, m, k;
  int s, q;
  scanf("%d %d %d", &n, &m, &k);
  double** coord = (double **) malloc(n*sizeof(double *));
  for(int i = 0; i<n; i++){
    coord[i] = (double *) malloc(2*sizeof(double));
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
    int v1, v2;
    scanf("%d %d", &v1, &v2);
    addEdge(graph, v1, v2, 0);
    addEdge(graph, v1, v2, 1);
  }
  scanf("%d %d", &s, &q);

  printGraph(graph);

  double * distances = (double *)malloc(graph->size * sizeof(double));
  dijkstra(graph, distances, coord, q);

  for(int i = 0; i<graph->size; i++){
    printf("%.4f\n", distances[i]);
  }


  //deallocate memory
  free(distances);
  destroyGraph(graph);
  for(int i = 0; i<n; i++){
    free(coord[i]);
  }
  free(coord);

  return 0;
  
}