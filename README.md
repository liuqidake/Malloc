# Malloc

A class that has 2 public methods: `void* allocate(size_t bytesToAllocate)` and `void deallocate(void* ptr)`. that are near drop-in replacements for `malloc` and `free`.

In addition, the implementation had the following other methods:

* Constructor/Destructor
* hash table insert, delete, grow

The allocate function should essentially

* round up the allocation size to the next multiple of the page size
* allocate memory with mmap
* insert the returned pointer and the allocation size in the hash table

The deallocate function should do the inverse
