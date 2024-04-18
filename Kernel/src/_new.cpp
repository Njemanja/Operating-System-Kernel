//
// Created by marko on 20.4.22..
//

#include "../lib/mem.h"
#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"
using size_t = decltype(sizeof(0));






void *operator new(size_t n)
{
    return MemoryAllocator::mem_alloc(n);   // mem_alloc(c);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem__free(p);

}

void operator delete[](void *p) noexcept
{
mem_free(p);
}