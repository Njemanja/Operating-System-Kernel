#include "../h/MemoryAllocator.hpp"
#include "../javniTestovi_2023_1_1/printing.hpp"
memList MemoryAllocator::free;
memList MemoryAllocator::used;

void *MemoryAllocator::mem_alloc(size_t size) {
    size_t block_cnt = (size + sizeof(MemoryElement))/MEM_BLOCK_SIZE;
    if(size%MEM_BLOCK_SIZE > 0){
        block_cnt+=1;
    }
    size_t actuall_size = (size_t)(block_cnt * MEM_BLOCK_SIZE);
    MemoryElement* free_cell = free.getFirst(actuall_size);
    if(free_cell == nullptr) return nullptr;
    const void* start_adr = (const void*)((char*)free_cell->start_adr - sizeof(MemoryElement));
    if (free_cell->size != actuall_size){
        MemoryElement * new_free_cell = (MemoryElement *) ((char*)start_adr + actuall_size);
        new_free_cell->size = free_cell->size - actuall_size;
        new_free_cell->start_adr = (const void*)((char*)start_adr + actuall_size + sizeof(MemoryElement));
        new_free_cell->next = nullptr;
        //MemoryElement* new_free_cell = new MemoryElement(free_cell->size - actuall_size, (const void*)((char*)start_adr + actuall_size));
        free.add(new_free_cell); //add ne dodaje prev
    }
    free.remove(free_cell->start_adr); //treba da postavlja na null
    MemoryElement * used_cell = (MemoryElement *) start_adr;
    used_cell->next= nullptr;
    used_cell->prev= nullptr;
    used_cell->size=actuall_size;
    used_cell->start_adr = (const void*)((char*)start_adr + sizeof(MemoryElement));
    used.add(used_cell);

    return (void*)used_cell->start_adr;
}

int MemoryAllocator::mem_free(void *adr) {

    MemoryElement* used_cell = used.get(adr);
    if (!used_cell) return -1;
    used.remove(adr);

    MemoryElement * new_free = (MemoryElement *) ((char*)used_cell->start_adr - sizeof(MemoryElement));
    new_free->size=used_cell->size;
    new_free->start_adr = (const void*)((char*)used_cell->start_adr - sizeof(MemoryElement));

    //MemoryElement* new_free = new MemoryElement(used_cell->size, used_cell->start_adr);
    MemoryElement* next_free = free.getNext(adr);
    MemoryElement* prev_free = free.getPrev(adr);
    if(next_free && (const void*)((char*)adr + used_cell->size) == next_free->start_adr){
        new_free->size += next_free->size;
        new_free->next = next_free->next;
        free.remove(next_free->start_adr);
    }
    else{
        new_free->next = next_free;
    }
    if(prev_free && (const void*)((char*)prev_free->start_adr + prev_free->size) == adr){
      prev_free->size += new_free->size;
      prev_free->next = new_free->next;
      //delete new_free;
    }
    else{
        new_free->prev = prev_free;
        free.add(new_free);
    }
    return 0;
}

void MemoryAllocator::initMemoryAllocator() {

    size_t sizee = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR);
    MemoryElement * heap = (MemoryElement *) HEAP_START_ADDR;
    heap->size=sizee;
    heap->start_adr = (const void*) ((char*)HEAP_START_ADDR + sizeof(MemoryElement)); //mozda plus velicina headera?
    //MemoryElement* heap = new MemoryElement(sizee, HEAP_START_ADDR);
    free.add(heap);
}