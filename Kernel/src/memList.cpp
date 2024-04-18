//
// Created by os on 8/8/23.
//
#include "../h/memList.hpp"
#include "../javniTestovi_2023_1_1/printing.hpp"
int size=0;
void memList::add(MemoryElement *element) {

    size++;
    MemoryElement* cur = head;
    if(!cur){
        head = element;
        tail = head;
        return;
    }else{
        element->prev=tail;
        tail->next=element;
        tail=element;
    }
    /*while(cur){
        if(cur->start_adr > element->start_adr) break;
        cur = cur->next;
    }

    if(!cur) {
        tail->next = element;
        element->prev = tail;
        tail = element;
    }
    else{

        cur->prev->next = element;
        element->next = cur;
        element->prev = cur->prev;
        cur->prev = element;
    }*/
}

int memList::remove(const void* adr) {

    MemoryElement* cur = head;
    while(cur){
        if(cur->start_adr == adr) break;
        cur = cur->next;
    }
    if(!cur) return -1;
    if(cur->prev){
        cur->prev->next = cur->next;
    }else{
        head=cur->next;
        cur->next->prev = nullptr;
    }
    if(cur->next){
        cur->next->prev = cur->prev;
    }else{
        tail=cur->prev;
        cur->prev->next = nullptr;
    }

    delete cur; //*
    return 0;
}

MemoryElement* memList::get(const void *adr) {
    MemoryElement* cur = head;
    int i=0;
    while(cur){
        i++;
        if(i>size) return nullptr;
        if(cur->start_adr == adr) break;
        cur = cur->next;
    }
    if(!cur) return nullptr;
    return cur;
}

MemoryElement *memList::getNext(const void *adr) {
    MemoryElement* cur = head;
    while(cur){
        if(cur->start_adr > adr) break;
        cur = cur->next;
    }
    if(!cur) return nullptr;
    return cur;
}

MemoryElement *memList::getPrev(const void *adr) {
    MemoryElement* cur = head;
    while(cur){
        if(cur->start_adr > adr) break;
        cur = cur->next;
    }
    if(!cur) return nullptr;
    return cur->prev;
}

MemoryElement *memList::getFirst(size_t size) {
    MemoryElement* cur = head;
    while(cur){
        if(cur->size >= size) break;
        cur = cur->next;
    }
    if(!cur) return nullptr;
    return cur;
}