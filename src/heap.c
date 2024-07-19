#include <stdlib.h>
#include "heap.h"


Path * pathNew(int vertex, int distance, Path * predecessor){
  Path * path = (Path *)malloc(sizeof(Path));
  path->distance = distance;
  path->vertex = vertex;
  path->predecessor = predecessor;
  return path;
}

Heap* heapNew(int initalCapacity){
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
  int inital = h->capacity;
  h->capacity = 2*h->capacity;
  h->paths = (Path **)realloc(h->paths, h->capacity*sizeof(Path*));
  for(int i = inital; i<h->capacity; i++){
    h->paths[i] = NULL;
  }
}


void heapDelete(Heap* h){
  free(h->paths);
  free(h);
}

void heapInsert(Heap* h, int vertex, int distance, Path * predecessor){
  if(h->size >= h->capacity - 1){
    heapResize(h);
  }
  int pos = h->size;
  h->paths[pos] = pathNew(vertex, distance, predecessor);
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
  Path * x = h->paths[0];
  h->paths[0] = h->paths[h->size-1];
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
    }else if(h->paths[posEsq] < h->paths[posDir]){
      s = posEsq;
    }else{
      s = posDir;
    }
    if(h->paths[i] > h->paths[s]){
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

int heapGetAncestor(Heap* h, int position){
  int ancPos = (position-1)/2;
  if(ancPos >= 0)
    return ancPos;
  else
    return -1;
}

int heapGetSuccessorLeft(Heap* h, int posicao){
  int sucEsq = 2*posicao+1;
  if(sucEsq < h->size){
    return sucEsq;
  }else{
    return -1;
  }
}

int heapGetSuccessorRight(Heap* h, int posicao){
  int sucDir = 2*posicao+2;
  if(sucDir < h->size){
    return sucDir;
  }else{
    return -1;
  }
}

int heapEmpty(Heap* h){
  if(h->size == 0){
    return 1;
  }

  return 0;
}
