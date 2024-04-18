#include "../h/syscall_c.hpp"
#include "../lib/mem.h"



void* mem_alloc(size_t size){

    uint64 c_int=(uint64)size;
    __asm__ volatile ("mv a1, %0" : : "r" (c_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x01));
    __asm__ volatile ("ecall");
    uint64 c1_int;
    __asm__ volatile ("mv %0, a0" : "=r" (c1_int));
    return (void* )c1_int;


    //return nullptr;
   //return __mem_alloc(size);
}
int mem_free(void* ptr){
    uint64 c_int=(uint64)ptr;
   __asm__ volatile ("mv a1, %0" : : "r" (c_int));
   __asm__ volatile ("mv a0, %0" : : "r" (0x02));
   __asm__ volatile ("ecall");
   return 0;


   // __mem_free(ptr);
    return 0;
}


//KONZOLA ----------------------------------------------------------------------------------------------------------------------------
void putc(char c){
    uint64 c_int=(uint64)c;
    __asm__ volatile ("mv a1, %0" : : "r" (c_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x42));
    __asm__ volatile ("ecall");
}

char getc(){

    __asm__ volatile ("mv a0, %0" : : "r" (0x41));
    __asm__ volatile ("ecall");
    char c_int;
    __asm__ volatile ("mv %0, a0" : "=r" (c_int));
    char c=(char)c_int;
    return c;

}

int time_sleep(time_t time){
    if(!time){
        return -1;
    }
    uint64 time_int=(uint64)time;
    __asm__ volatile ("mv a1, %0" : : "r" (time_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x31));
    __asm__ volatile ("ecall");

    return 0;
}


//SEMAFORI ----------------------------------------------------------------------------------------------------------------------------
int sem_open(sem_t* handle,unsigned  init){
    uint64 init_u=(uint64)init;
    if((int)init<0){
        return -1;
    }
    uint64 id=(uint64)handle;
    __asm__ volatile ("mv a2, %0" : : "r" (id));
    __asm__ volatile ("mv a1, %0" : : "r" (init_u));
    __asm__ volatile ("mv a0, %0" : : "r" (0x21));
    __asm__ volatile ("ecall");

    return 0;

}
int sem_close(sem_t id){
    if(!id){
        return -1;
    }
    uint64 id_int=(uint64)id;
    __asm__ volatile ("mv a1, %0" : : "r" (id_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x22));
    __asm__ volatile ("ecall");
    return 0;

}
int sem_wait(sem_t id){
    if(!id){
        return 0;
    }
    uint64 id_int=(uint64)id;
    __asm__ volatile ("mv a1, %0" : : "r" (id_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x23));
    __asm__ volatile ("ecall");
    return 0;

}
int sem_signal(sem_t id){

    uint64 id_int=(uint64)id;
    __asm__ volatile ("mv a1, %0" : : "r" (id_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x24));
    __asm__ volatile ("ecall");
    return 0;
}




//NITI ----------------------------------------------------------------------------------------------------------------------------
int thread_create(thread_t* handle, void (*startroutine)(void *), void* arg){

    /*uint64 args_int=(uint64)arg;
    __asm__ volatile ("mv a3, %0" : : "r" (args_int));*/
    uint64 args_int=(uint64)arg;
    __asm__ volatile ("mv a7, %0" : : "r" (args_int));


    uint64 startroutine_int=(uint64)startroutine;
    __asm__ volatile ("mv a2, %0" : : "r" (startroutine_int));

    uint64 handle_int=(uint64)handle;
    __asm__ volatile ("mv a1, %0" : : "r" (handle_int));


    __asm__ volatile ("mv a0, %0" : : "r" (0x11));
    __asm__ volatile ("mv a6, %0" : : "r" (1));
    __asm__ volatile ("ecall");

    uint64 return_int;
    __asm__ volatile ("mv %0, a0" : "=r" (return_int));
    handle=(thread_t*)return_int;

    return 0;
}

int thread_exit(){
    __asm__ volatile ("mv a0, %0" : : "r" (0x12));
    __asm__ volatile ("ecall");
    uint64 value_int;
    __asm__ volatile ("mv %0, a0" : "=r" (value_int));
    int value=(char)value_int;
    return value;
}

void thread_dispatch(){
    __asm__ volatile ("mv a0, %0" : : "r" (0x13));
    __asm__ volatile ("ecall");
}

void thread_join(thread_t handle){

    uint64 handle_int=(uint64)handle;
    __asm__ volatile ("mv a1, %0" : : "r" (handle_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x14));
    __asm__ volatile ("ecall");
}

void thread_start(thread_t handle){

    uint64 handle_int=(uint64)handle;
    __asm__ volatile ("mv a1, %0" : : "r" (handle_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x15));
    __asm__ volatile ("ecall");
}

int thread_make(thread_t* handle, void (*startroutine)(void *), void* arg){
    uint64 args_int=(uint64)arg;
    __asm__ volatile ("mv a7, %0" : : "r" (args_int));

    uint64 startroutine_int=(uint64)startroutine;
    __asm__ volatile ("mv a2, %0" : : "r" (startroutine_int));

    uint64 handle_int=(uint64)handle;
    __asm__ volatile ("mv a1, %0" : : "r" (handle_int));



    __asm__ volatile ("mv a0, %0" : : "r" (0x11));
    __asm__ volatile ("mv a6, %0" : : "r" (0));

    __asm__ volatile ("ecall");

    uint64 return_int;
    __asm__ volatile ("mv %0, a0" : "=r" (return_int));
    handle=(thread_t*)return_int;
    return 0;

}

void thread_delete(thread_t* handle){
    uint64 handle_int=(uint64)handle;
    __asm__ volatile ("mv a2, %0" : : "r" (handle_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x16));
    __asm__ volatile ("ecall");
}

int mem__free(void* ptr){
    mem_free(ptr);
    /*uint64 c_int=(uint64)ptr;
    __asm__ volatile ("mv a1, %0" : : "r" (c_int));
    __asm__ volatile ("mv a0, %0" : : "r" (0x02));
     __asm__ volatile ("ecall");*/
    return 0;

}