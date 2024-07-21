#include "heap.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

double euclideanDistance(double ** coord, int initial, int goal){
  /*
  Function to calculate the euclidean distance between two vertices
  */
  double x = coord[goal][0];
  double y = coord[goal][1];
  double x_init = coord[initial][0];
  double y_init = coord[initial][1];

  return sqrt(pow(x-x_init, 2) + pow(y-y_init, 2));
}

void A_star(Graph * graph, double (*heuristic)(double**, int, int), double * g_score, double ** coord, int maxPortals){
  /*
  A* implementation with custom heuristic it updates the distances (g_score), but terminates if the
  last node is reached
  */
  Heap * minHeap = heapNew(graph->totalPaths + graph->totalPortals); // creating a heap with capacity equal to the number of edges in the graph
  int visited[graph->size]; // is garanteed that if I visited a vertex it will be by the shortest path
  int goal = graph->size -1;
  memset(visited, 0, sizeof(visited));
  for (size_t i = 0; i<graph->size; i++){
    g_score[i] = DBL_MAX;
  }

  heapInsert(minHeap, 0, 0, 0);
  g_score[0] = 0;

  while(heapEmpty(minHeap) != 1){
    Path * path = heapRemove(minHeap);
    if(path->portals > maxPortals){
      free(path);
      continue;
    }
    visited[path->vertex] = 1;
    Node * node = graph->adjList[path->vertex];
    while(node != NULL){
      if(visited[node->vertex] == 1){
        node = node->next;
        continue;
      }
      double tentative_g_score = g_score[path->vertex] + euclideanDistance(coord, path->vertex, node->vertex);
      if(node->portal == 1){
        tentative_g_score = g_score[path->vertex];
      }
      if(tentative_g_score >= g_score[node->vertex]){
        node = node->next;
        continue;
      }
      int portals = node->portal + path->portals;
      double tentative_f_score = tentative_g_score + heuristic(coord, node->vertex, goal);
      heapInsert(minHeap, node->vertex, tentative_f_score, portals);
      g_score[node->vertex] = tentative_g_score;
      node = node->next;
    }
    free(path);
  }
  heapDestroy(minHeap);
}

void dijkstra(Graph * graph, double * distances, double ** coord, int maxPortals){
  /*
  dijkstra custom approach it updates the distances vector
  */
  for (size_t i = 0; i<graph->size; i++){
    distances[i] = DBL_MAX;
  }
  Heap * minHeap = heapNew(graph->totalPaths + graph->totalPortals); // creating a heap with capacity equal to the number of edges in the graph
  heapInsert(minHeap, 0, 0, 0);
  while(heapEmpty(minHeap) == 0){
    Path * path = heapRemove(minHeap);
    if(distances[path->vertex] < path->distance || path->portals > maxPortals){
      free(path);
      continue;
    }
    distances[path->vertex] = path->distance;
    Node * node = graph->adjList[path->vertex];
    while(node != NULL){
      double d = path->distance + euclideanDistance(coord, path->vertex, node->vertex);
      if(node->portal == 1){
        d = path->distance;
       }
      int portals = node->portal + path->portals;
      heapInsert(minHeap, node->vertex, d, portals);
      node = node->next;
    }
    free(path);
  }
  heapDestroy(minHeap);
}

int main(){
  int n, m, k;
  double s;
  int q;
  scanf("%d %d %d", &n, &m, &k);
  double** coord = (double **) malloc(n*sizeof(double *)); // create a coordinates matrix
  for(int i = 0; i<n; i++){
    coord[i] = (double *) malloc(2*sizeof(double)); //take the coordinates from user
  }
  Graph * graph = createGraph(n, m, k);

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
  scanf("%lf %d", &s, &q);

  double * distances = (double *)malloc(graph->size * sizeof(double)); //creating the distances vector
  dijkstra(graph, distances, coord, q);
  int dijkstra_result = (distances[graph->size-1] <= s) ? 1 : 0; // simple test if the last distance is less than energy
  A_star(graph, euclideanDistance, distances, coord, q);
  int a_star_result = (distances[graph->size-1] <= s) ? 1 : 0; // simple test if the last distance is less than energy
  
  //print result
  printf("%d %d\n", dijkstra_result, a_star_result);

  //deallocate memory
  free(distances);
  destroyGraph(graph);
  for(int i = 0; i<n; i++){
    free(coord[i]);
  }
  free(coord);

  return 0;
  
}