//
// Created by os on 8/8/23.
//

#ifndef PROJECT_BASE_V1_1_MEMLIST_H
#define PROJECT_BASE_V1_1_MEMLIST_H
#include "../lib/hw.h"
typedef struct MemoryElement{
    MemoryElement* prev = nullptr;
    MemoryElement* next = nullptr;
    size_t size;
    const void* start_adr;
    MemoryElement(size_t s, const void* adr): size(s), start_adr(adr){}
} MemoryElement;

class memList{
public:
    MemoryElement* head = nullptr;
    MemoryElement* tail = nullptr;
    MemoryElement* get(const void* adr);
    MemoryElement* getNext(const void* adr);
    MemoryElement* getPrev(const void* adr);
    int remove(const void* adr);
    void add(MemoryElement* element);
    MemoryElement* getFirst(size_t size);
};

#endif //PROJECT_BASE_V1_1_MEMLIST_H
