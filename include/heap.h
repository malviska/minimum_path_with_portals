#ifndef HEAP_HPP
#define HEAP_HPP
typedef struct Heap{
    int capacity;
    int size;
    struct Path ** paths;
} Heap;

typedef struct Path {
  int vertex;
  int distance;
  struct Path * predecessor;
} Path;


Path * pathNew(int vertex, int distance, Path * predecessor);

Heap* heapNew(int initialCapacity);
void heapResize(Heap * h);
void heapDelete(Heap* h);

void heapInsert(Heap* h, int x);
int heapRemove(Heap* h);

int heapGetAncestor(Heap* h, int posicao);
int heapGetSuccessorLeft(Heap* h, int posicao);
int heapGetSuccessorRight(Heap* h, int posicao);

int heapEmpty(Heap* h);


#endif