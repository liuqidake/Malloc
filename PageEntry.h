//
// Created by Qi Liu on 2019-02-26.
//

#include <iostream>

#ifndef MSDALLOC_PAGEENTRY_H
#define MSDALLOC_PAGEENTRY_H

struct PageEntry{
    void *pointer;
    size_t space;
};

#endif //MSDALLOC_PAGEENTRY_H
