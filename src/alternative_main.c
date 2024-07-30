/*
    Alternative implementation with matrix of adjacency
*/
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "config.h"

void clkDiff(struct timespec t1, struct timespec t2,
                   struct timespec * res)
// Descricao: calcula a diferenca entre t2 e t1, que e armazenada em res
// Entrada: t1, t2
// Saida: res
{
  if (t2.tv_nsec < t1.tv_nsec){
    // ajuste necessario, utilizando um segundo de tv_sec
    res-> tv_nsec = 1000000000+t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec-1;
  } else {
    // nao e necessario ajuste
    res-> tv_nsec = t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec;
  }
}
typedef struct Edge {
  /*
   In a matrix of adjacency every unit represents a edge and their characteristics
  */
  int path; 
  int portal;

} Edge;

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

void A_star(Edge ** graph, int vertices, double (*heuristic)(double**, int, int), double * g_score, double ** coord, int maxPortals){
  /*
  A* implementation with custom heuristic it updates the distances (g_score), but terminates if the
  last node is reached
  */
  Heap * minHeap = heapNew(2*vertices); // creating a heap with capacity equal to the number of edges in the graph
  int visited[vertices]; // it is guaranteed that if I visit a vertex, it will be via the shortest path.
  int goal = vertices -1; // last vertex
  memset(visited, 0, sizeof(visited));
  g_score[0] = 0; // distance from 0 to 0 is 0
  for (size_t i = 1; i<vertices; i++){
    g_score[i] = DBL_MAX; // Initialize the distances as the farthest possible (Infinity)
  }

  heapInsert(minHeap, 0, 0, 0); // insert initial vertex with weight 0

  while(heapEmpty(minHeap) != 1){
    Path * path = heapRemove(minHeap);
    if(path->vertex == goal){
      free(path);
      break;
    }
    visited[path->vertex] = 1;
    for(int i = 0; i<vertices; i++){
      Edge node = graph[path->vertex][i];
      if(visited[i] == 1 || node.path == 0){
        continue;
      }
      double tentative_g_score = g_score[path->vertex] + euclideanDistance(coord, path->vertex, i); // calculate distance to the neightbour and sum to the total distance
      if(node.portal == 1){
        tentative_g_score = g_score[path->vertex]; // if is a neightbour by portal transverse the distance from vertex to neightbour is 0
      }
      int portals = node.portal + path->portals; // total portals transversed
      if(tentative_g_score >= g_score[i] || portals > maxPortals){ // if distance is bigger than previously one or number of portals surpasses the maximum number jump this node
        continue;
      }
      double tentative_f_score = tentative_g_score + heuristic(coord, i, goal); // weight is the sum of the distance and the heuristic
      heapInsert(minHeap, i, tentative_f_score, portals);
      g_score[i] = tentative_g_score; // with all tests passes update the distance
    }
    free(path);
  }
  heapDestroy(minHeap);
}

void dijkstra(Edge ** graph, int vertices, double * distances, double ** coord, int maxPortals){
  /*
  Dijkstra implementation with portals and problem specific implementation, it terminates as soon as 
  the program arrives to the last vertice (goal)
  */

  int goal = vertices -1; // last vertex
  int visited[vertices]; // it is guaranteed that if I visit a vertex, it will be via the shortest path.
  memset(visited, 0, sizeof(visited));
  distances[0] = 0; // distance from 0 to 0 is 0
  for (size_t i = 1; i<vertices; i++){
    distances[i] = DBL_MAX; // Initialize the distances as the farthest possible (Infinity)
  }
  Heap * minHeap = heapNew(2*vertices); // creating a heap with capacity equal to the number of edges in the graph
  heapInsert(minHeap, 0, 0, 0); // insert initial vertex with weight 0
  while(heapEmpty(minHeap) == 0){
    Path * path = heapRemove(minHeap);
    if(path->vertex == goal){
      free(path);
      break;
    }
    visited[path->vertex] = 1;
    for(int i = 0; i<vertices; i++){
      Edge node = graph[path->vertex][i];
      if(visited[i] == 1 || node.path == 0){
        continue;
      }
      double d = path->distance + euclideanDistance(coord, path->vertex, i); // calculate distance to the neightbour and sum to the total distance
      if(node.portal == 1){
        d = path->distance; // if is a neightbour by portal transverse the distance from vertex to neightbour is 0
      }
      int portals = node.portal + path->portals; // total portals transversed
      if(d >= distances[i] || portals > maxPortals){
        continue;
      }
      heapInsert(minHeap, i, d, portals);
      distances[i] = d; // with all tests passes update the distance
    }
    free(path);
  }
  heapDestroy(minHeap);
}

int main(){
  resetGlobals();
  struct timespec inittp, endtp, restp;
  int n, m, k;
  double s;
  int q;
  scanf("%d %d %d", &n, &m, &k);
  allocs++;
  memoryUsage += n*sizeof(double *);
  double** coord = (double **) malloc(n*sizeof(double *)); // create a coordinates matrix
  for(int i = 0; i<n; i++){
    allocs++;
    memoryUsage += 2*sizeof(double);
    coord[i] = (double *) malloc(2*sizeof(double)); //take the coordinates from user
  }
  allocs++;
  memoryUsage += n*sizeof(Edge *);
  Edge ** graph = (Edge **) malloc(n*sizeof(Edge *)); // matrix of adjacency
  for(int i = 0; i<n; i++){
    allocs++;
    memoryUsage += n*sizeof(Edge);
    graph[i] = (Edge *) calloc(n ,sizeof(Edge)); // represented by a full matrix since is a directed graph
  }

  for(int i = 0; i<n; i++){
    double x, y;
    scanf("%lf %lf", &x, &y);
    coord[i][0] = x;
    coord[i][1] = y;
  }
  for(int i = 0; i<m; i++){
    int v1, v2;
    scanf("%d %d", &v1, &v2);
    graph[v1][v2].path = 1;
  }
  for(int i = 0; i<k; i++){
    int v1, v2;
    scanf("%d %d", &v1, &v2);
    graph[v1][v2].path = 1;
    graph[v1][v2].portal = 1;
  }
  scanf("%lf %d", &s, &q);
  allocs++;
  memoryUsage += n * sizeof(double);
  double * distances = (double *)malloc(n * sizeof(double)); //creating the distances vector

  clock_gettime(CLOCK_MONOTONIC, &inittp);
  dijkstra(graph, n, distances, coord, q);
  clock_gettime(CLOCK_MONOTONIC, &endtp);
  int dijkstra_result = (distances[n-1] <= s) ? 1 : 0; // simple test if the last distance is less than energy
  clkDiff(inittp, endtp, &restp);
  long dijkstra_sec = restp.tv_sec;
  long dijkstra_nsec = restp.tv_nsec;
  double dijkstra_distance = distances[n-1];

  clock_gettime(CLOCK_MONOTONIC, &inittp);
  A_star(graph, n, euclideanDistance, distances, coord, q);
  clock_gettime(CLOCK_MONOTONIC, &endtp);
  int a_star_result = (distances[n-1] <= s) ? 1 : 0; // simple test if the last distance is less than energy
  clkDiff(inittp, endtp, &restp);
  long a_star_sec = restp.tv_sec;
  long a_star_nsec = restp.tv_nsec;
  double a_star_distance = distances[n-1];
  printf("matrix,%d,%d,%d,%ld.%.9ld,%.4lf,%ld.%.9ld,%.4lf,%ld,%ld\n", n, m, k, dijkstra_sec, dijkstra_nsec, dijkstra_distance, a_star_sec, a_star_nsec, a_star_distance, allocs, memoryUsage);
  
  //print result
  //printf("%d %d\n", dijkstra_result, a_star_result);

  //deallocate memory
  free(distances);
  for(int i = 0; i<n; i++){
    free(graph[i]);
  }
  free(graph);
  for(int i = 0; i<n; i++){
    free(coord[i]);
  }
  free(coord);

  return 0;
  
}