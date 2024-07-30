#include "config.h"

size_t allocs = 0;
size_t memoryUsage = 0;

void resetGlobals(){
  allocs = 0;
  memoryUsage = 0;
}