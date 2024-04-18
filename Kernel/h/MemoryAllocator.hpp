
#include "../lib/hw.h"
#include "memList.hpp"









class MemoryAllocator {
private:
    static memList free;
    static memList used;
public:
    static void initMemoryAllocator();
    static  void* mem_alloc(size_t);
    static int mem_free(void*);

};





