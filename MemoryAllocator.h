//
// Created by Qi Liu on 2019-02-25.
//

#ifndef MSDALLOC_MEMORYALLOCATOR_H
#define MSDALLOC_MEMORYALLOCATOR_H

#include "PageEntry.h"
#include <sys/mman.h>
#include <cmath>
#include <mach/vm_types.h>

class MemoryAllocator{
private:
    PageEntry *table;
    int size;
    int capacity;
public:
    MemoryAllocator();
    void* allocate(size_t bytesToAllocate);
    void deallocate(void* ptr);
    //for test
    PageEntry get(void *ptr);
    int getSize();
private:
    bool insert(const PageEntry &pageEntry);
    bool remove(void *ptr);
    void grow();

};
#endif //MSDALLOC_MEMORYALLOCATOR_H
