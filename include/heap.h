#ifndef HEAP_HPP
#define HEAP_HPP
typedef struct Heap{
  /*
  Simple minimum Heap structure with dynamic allocation to store the paths and prioritizes the shortest ones
  */
  int capacity;
  int size;
  struct Path ** paths;
} Heap;

typedef struct Path {
   /*
  Path structure stores the number of the vertice, the distance travelled until there and the number
  of portals taken
  */
  int vertex;
  double distance;
  int portals;
} Path;


Path * pathNew(int vertex, double distance, int portals); // create a path
Heap* heapNew(int initialCapacity); // create a heap
void heapResize(Heap * h); // resize the heap (dynamic allocation)
void heapInsert(Heap* h,int vertex, double distance, int portals); // insert a path
Path * heapRemove(Heap* h); // remove and return the shortest path
int heapGetAncestor(Heap* h, int position); // get the closest ancestor of the path return -1 with there is no ancestor
int heapGetSuccessorLeft(Heap* h, int position); // get the left successor of the path return -1 with there is none
int heapGetSuccessorRight(Heap* h, int position); // get the right successor of the path return -1 with there is none
int heapEmpty(Heap* h); // return 1 with the heap is empty 0 otherwise
void heapDestroy(Heap * heap); // deallocate the heap
void heapPrint(Heap * heap); // print the heap (util for debugging)


#endif