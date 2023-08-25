#ifndef VMALLOC_H
#define VMALLOC_H

#include "../include.h"

volatile bool AllocIdMap(uint64_t Addr, uint64_t Size);
volatile bool AllocMap(uint64_t vAddr, uint64_t pAddr, uint64_t Size);
volatile void AllocUnMap(uint64_t vAddr, uint64_t Size);
volatile void AllocInit();

#endif VMALLOC_H