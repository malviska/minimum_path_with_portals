#ifndef GRAPH_H
#define GRAPH_H

typedef struct Node {
  /*
  chained list that stores the vertex and if it is linked through a portal, points to the next element
  */
  int vertex;
  int portal; // 0 or 1
  struct Node* next;
} Node;

typedef struct Graph {
  /*
  simple graph structure that uses an adjacency list to store the vertices and their neightbours
  */
  int size;
  Node ** adjList;
  int totalPaths;
  int totalPortals;
} Graph;

Node * createNode(int vertex, int portal); // create an element of a list
Graph* createGraph(int size, int totalPaths, int totalPortals); // create a new graph
void destroyGraph(Graph* graph); // deallocate the graph from the memory
void addEdge(Graph* graph, int src, int dest, int portal); // add an edge to a specific vertex of the graph
void printGraph(Graph* graph); // print the graph most for debugging pourposes



#endif