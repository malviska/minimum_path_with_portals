#include<stdlib.h>
#include<stdio.h>
#include"graph.h"

Node * createNode(int vertex, int portal){
  Node * node = (Node *)malloc(sizeof(Node));
  node->vertex = vertex;
  node->portal = portal;
  node->next = NULL;

  return node;
}

Graph * createGraph(int size){
  Graph * graph = (Graph *)malloc(sizeof(Graph));
  graph->size = size;
  graph->adjList = (Node **)malloc(size*sizeof(Node*));

  for(int i=0; i<size; i++){
    graph->adjList[i] = NULL;
  }

  return graph;
}

void addEdge(Graph* graph, int src, int dst, int portal){
  Node * newNode = createNode(dst, portal);
  newNode->next = graph->adjList[src];
  graph->adjList[src] = newNode;
}

void destroyGraph(Graph* graph){
  for(int i = 0; i< graph->size; i++){
    Node * node = graph->adjList[i];
    while(node != NULL){
      Node * aux = node->next;
      free(node);
      node = aux;
    }
  }
  free(graph->adjList);
  free(graph);
}

void printGraph(Graph* graph){
  for(int i = 0; i< graph->size; i++){
    printf("%d: ", i);
    Node * node = graph->adjList[i];
    while(node != NULL){
      printf("%d ", node->vertex);
      node = node->next;
    }
    printf("\n");
  }
}