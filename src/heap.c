#include <stdio.h>
#include <stdlib.h>
#include "heap.h"


Path * pathNew(int vertex, double distance, int portals){
  /*
   Create a new path unit that represents a edge in a heap, based on the edge characteristics
  */
  Path * path = (Path *)malloc(sizeof(Path));
  path->distance = distance;
  path->vertex = vertex;
  path->portals = portals;
  return path;
}

Heap* heapNew(int initalCapacity){
  /*
   Create a new heap with initial capacity given but dynamic allocated
  */
  Heap * minHeap = (Heap *)malloc(sizeof(Heap));
  minHeap->capacity = initalCapacity;
  minHeap->size = 0;
  minHeap->paths = (Path **)malloc(initalCapacity * sizeof(Path*));

  for(int i = 0; i<initalCapacity; i++){
    minHeap->paths[i] = NULL;
  }

  return minHeap;
}

void heapResize(Heap * h){
  /*
    Dynamic allocate the heap
  */
  int inital = h->capacity;
  h->capacity = 2*h->capacity;
  h->paths = (Path **)realloc(h->paths, h->capacity*sizeof(Path*));
  for(int i = inital; i<h->capacity; i++){
    h->paths[i] = NULL;
  }
}

void heapInsert(Heap* h, int vertex, double distance, int portals){
  /*
   Insert new path and heapfy considering the minimum distance
  */
  if(h->size + 1 >= h->capacity - 1){
    heapResize(h);
  }
  int pos = h->size;
  h->paths[pos] = pathNew(vertex, distance, portals);
  int ancPos = heapGetAncestor(h, pos);
  while(h->paths[pos]->distance < h->paths[ancPos]->distance && ancPos != -1){
    Path * aux = h->paths[ancPos];
    h->paths[ancPos] = h->paths[pos];
    h->paths[pos] = aux;
    pos = ancPos;
    ancPos = heapGetAncestor(h, pos);
  }
  h->size++;
}

Path * heapRemove(Heap* h){
  /*
   Remove top from heap and return it, top is the minimum distance path
  */
  Path * x = h->paths[0];
  h->paths[0] = h->paths[h->size-1];
  h->paths[h->size-1] = NULL;
  h->size--;
  int i = 0;
  int s;
  int posEsq = heapGetSuccessorLeft(h, 0);
  int posDir = heapGetSuccessorRight(h, 0);
  while(posDir != -1 || posEsq != -1){
    if(posDir == -1){
      s = posEsq;
    }else if(posEsq == -1){
      s = posDir;
    }else if(h->paths[posEsq]->distance < h->paths[posDir]->distance){
      s = posEsq;
    }else{
      s = posDir;
    }
    if(h->paths[i]->distance > h->paths[s]->distance){
      Path * aux = h->paths[s];
      h->paths[s] = h->paths[i];
      h->paths[i] = aux;
      i = s;
    }else{
      break;
    }
    posEsq = heapGetSuccessorLeft(h, i);
    posDir = heapGetSuccessorRight(h, i);
  }

  return x;
}

// helper functions
int heapGetAncestor(Heap* h, int position){
  int ancPos = (position-1)/2;
  return (ancPos >= 0) ? ancPos : -1;
}

int heapGetSuccessorLeft(Heap* h, int position){
  int sucEsq = 2*position+1;
  return (sucEsq < h->size) ? sucEsq : -1;
}

int heapGetSuccessorRight(Heap* h, int position){
  int sucDir = 2*position+2;
  return (sucDir < h->size) ? sucDir : -1;
}

int heapEmpty(Heap* h){
  return (h->size > 0) ? 0 : 1;
}

void heapPrint(Heap * h){
  for(int i = 0; i< h->size; i++){
    printf("%d %.4f % d\n", h->paths[i]->vertex, h->paths[i]->distance, h->paths[i]->portals);
  }
}

void heapDestroy(Heap * h){
  for(int i = 0; i< h->capacity; i++){
    free(h->paths[i]);
  }
  free(h->paths);
  free(h);
}
