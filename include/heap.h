#ifndef HEAP_HPP
#define HEAP_HPP
typedef struct Heap{
    int capacity;
    int size;
    struct Path ** paths;
} Heap;

typedef struct Path {
  int vertex;
  double distance;
  int portals;
} Path;


Path * pathNew(int vertex, double distance, int portals);

Heap* heapNew(int initialCapacity);
void heapResize(Heap * h);
void heapInsert(Heap* h,int vertex, double distance, int portals);
Path * heapRemove(Heap* h);
int heapGetAncestor(Heap* h, int position);
int heapGetSuccessorLeft(Heap* h, int position);
int heapGetSuccessorRight(Heap* h, int position);
int heapEmpty(Heap* h);
void heapDestroy(Heap * heap);
void heapPrint(Heap * heap);


#endif