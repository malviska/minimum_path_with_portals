#ifndef HEAP_HPP
#define HEAP_HPP

typedef struct s_heap{
    int tamanho;
    int* dados;
} Heap;

Heap* NovoHeap(int maxsize);
void DeletaHeap(Heap* h);

void Inserir(Heap* h, int x);
int Remover(Heap* h);

int GetAncestral(Heap* h, int posicao);
int GetSucessorEsq(Heap* h, int posicao);
int GetSucessorDir(Heap* h, int posicao);

//Retorna 1 caso h esteja vazio, 0 caso contrário.
int Vazio(Heap* h); 


#endif