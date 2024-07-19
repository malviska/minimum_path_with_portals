#ifndef GRAPH_H
#define GRAPH_H

typedef struct Node {
  int vertex;
  int portal; // 0 or 1
  struct Node* next;
} Node;

typedef struct Graph {
  int size;
  Node ** adjList;
} Graph;

Node * createNode(int vertex, int portal);
Graph* createGraph(int vertex);
void destroyGraph(Graph* graph);
void addEdge(Graph* graph, int src, int dest, int portal);
void printGraph(Graph* graph);



#endif