//
// Created by Qi Liu on 2019-02-25.
//

#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator(){
    capacity = 100;
    size = 0;
    table = (PageEntry *)mmap(nullptr, capacity * sizeof(PageEntry), PROT_WRITE|PROT_READ, MAP_ANONYMOUS| MAP_PRIVATE, -1, 0);
    if(table == MAP_FAILED) perror("Map failed");
}

void* MemoryAllocator::allocate(size_t bytesToAllocate) {
    if(bytesToAllocate<=0 ) return nullptr;
    //round up the allocation size to the next multiple of the page size
    if(bytesToAllocate % 16 != 0 ){
        int mod = bytesToAllocate % 16;
        bytesToAllocate += ( 16 - mod);
    }

    //std::cout<<bytesToAllocate<<std::endl;

    //Using mmap to allocate space
    void *pointer = mmap(nullptr, bytesToAllocate, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    if(pointer == MAP_FAILED){
        perror("Map failed");
    }

    //A pageEntry struct
    PageEntry entry = {pointer, bytesToAllocate};

    insert(entry);

    return pointer;
}

void MemoryAllocator::deallocate(void *ptr) {
    remove(ptr);
}

bool MemoryAllocator::insert(const PageEntry &pageEntry) {
    //check if the page entry is valid
    if(pageEntry.pointer == nullptr) return false;
    //get the hash value of this PageEntry
    int hashValue = (((unsigned long)pageEntry.pointer) >>12) % capacity;
    //std::cout<<(unsigned long)pageEntry.pointer<<std::endl;
    //check load factor
    if(size/capacity >= 0.5){
        grow();
    }

    int quad = 0;
    int counter = 0;
    while(true){
        int index = (hashValue + quad * quad)%capacity ;
        if((table+index)->pointer == nullptr){
            *(table+index) = pageEntry;
            size++;
            return true;
        }

        if(counter > 0 && index == hashValue ) break; //has cycle, cannot find a valid spot
        quad++;
        counter++;
    }
    return false;
}

void MemoryAllocator::grow()  {
    capacity = capacity * 2;
    size = 0;
    PageEntry *newTable = (PageEntry*)mmap(nullptr, capacity * sizeof(PageEntry), PROT_WRITE|PROT_READ, MAP_ANONYMOUS| MAP_PRIVATE, -1, 0);
    if(newTable == MAP_FAILED) perror("Rehash failed");
    for(int i = 0; i< capacity/2; i++){
        PageEntry current = *(table+i);
        if(current.pointer != nullptr && current.space != 0){
            int quad = 0;
            int counter = 0;
            int hashValue = (((unsigned long)current.pointer) >>12) % capacity;
            while(true){
                int index = (hashValue + quad * quad)%capacity ;
                if((newTable+index)->pointer == nullptr){
                    *(newTable+index) = current;
                    size++;
                    break;
                }

                if(counter > 0 && index == hashValue ) break; //has cycle, cannot find a valid spot
                quad++;
            }
        }
    }

    munmap(table, sizeof(PageEntry)*capacity/2);

    table = newTable;
}


bool MemoryAllocator::remove(void *ptr) {
    int hashValue = ((unsigned long)ptr >> 12) % capacity;
    int quad = 0;
    while(true){
        int index = (hashValue + quad*quad) % capacity;

        //found the ptr
        if((table + index) -> pointer == ptr){
            //dellocate the space
            munmap((table+index)->pointer, (table+index)->space);
            (table+index) -> pointer = nullptr;
            size--;
            return true;
        }

        //the ptr does not exist
        if((table+index) -> pointer == nullptr && (table+index)->space == 0){
            break;
        }

        quad++;
    }

    return false;

}


PageEntry MemoryAllocator::get(void *ptr){
    int hashValue = (((unsigned long)ptr) >>12) % capacity;
    //std::cout<< ((unsigned long)ptr) <<std::endl;
    int quad = 0;
    while(true){
        PageEntry entry = *(table+((hashValue + quad*quad)% capacity));
        if(entry.pointer == nullptr && entry.space == 0) break;
        if(entry.pointer == ptr) return entry;
        quad++;
    }
    return {nullptr, 0};
}

int MemoryAllocator::getSize() {return size;}