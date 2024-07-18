#include <stdlib.h>
#include "heap.h"

Heap* NovoHeap(int maxsize){
  Heap * minHeap = (Heap *)malloc(sizeof(Heap));
  minHeap->tamanho = 0;
  minHeap->dados = (int *)malloc(maxsize * sizeof(int));

  return minHeap;
}


void DeletaHeap(Heap* h){
  free(h->dados);
  free(h);
}

void Inserir(Heap* h, int x){
  int pos = h->tamanho;
  h->dados[pos] = x;
  int ancPos = GetAncestral(h, pos);
  while(h->dados[pos] < h->dados[ancPos] && ancPos != -1){
    int aux = h->dados[ancPos];
    h->dados[ancPos] = h->dados[pos];
    h->dados[pos] = aux;
    pos = ancPos;
    ancPos = GetAncestral(h, pos);
  }
  h->tamanho++;
}

int Remover(Heap* h){
  int x = h->dados[0];
  h->dados[0] = h->dados[h->tamanho-1];
  h->tamanho--;
  int i = 0;
  int s;
  int posEsq = GetSucessorEsq(h, 0);
  int posDir = GetSucessorDir(h, 0);
  while(posDir != -1 || posEsq != -1){
    if(posDir == -1){
      s = posEsq;
    }else if(posEsq == -1){
      s = posDir;
    }else if(h->dados[posEsq] < h->dados[posDir]){
      s = posEsq;
    }else{
      s = posDir;
    }
    if(h->dados[i] > h->dados[s]){
      int aux = h->dados[s];
      h->dados[s] = h->dados[i];
      h->dados[i] = aux;
      i = s;
    }else{
      break;
    }
    posEsq = GetSucessorEsq(h, i);
    posDir = GetSucessorDir(h, i);
  }

  return x;
}

int GetAncestral(Heap* h, int posicao){
  int ancPos = (posicao-1)/2;
  if(ancPos >= 0)
    return ancPos;
  else
    return -1;
}
int GetSucessorEsq(Heap* h, int posicao){
  int sucEsq = 2*posicao+1;
  if(sucEsq < h->tamanho){
    return sucEsq;
  }else{
    return -1;
  }
}
int GetSucessorDir(Heap* h, int posicao){
  int sucDir = 2*posicao+2;
  if(sucDir < h->tamanho){
    return sucDir;
  }else{
    return -1;
  }
}

//Retorna 1 caso h esteja vazio, 0 caso contrário.
int Vazio(Heap* h){
  if(h->tamanho == 0){
    return 1;
  }

  return 0;
}
