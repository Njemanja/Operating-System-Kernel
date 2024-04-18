#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/_sem.hpp"
#include "../lib/mem.h"
#include "../javniTestovi_2023_1_1/printing.hpp"
#include "../h/list.hpp"


//waitList _sem::waitlist;

int _sem::wait(sem_t sem) {
    if(sem->opened){

        if(sem->value==0){
            sem->waitlist.add(TCB::running);
            TCB::semDisp();
        }else{
            sem->value--;
        }
        return 0;
    }
    return -1;
}

int _sem::signal(sem_t sem) {
    if( sem->opened ){
       TCB* t=sem->waitlist.get();
       if(t== nullptr){
           sem->value++;
       }else{
           Scheduler::put(t);
       }
        return 0;
    }
    return -1;
}

int _sem::close(sem_t sem) {
    //printString("Semaphore close.\n");
    sem->opened=0;
    while(false){
        bool empty=sem->waitlist.empty();
        if(!empty){
            //sem->waitlist.release(&sem);
        }
        else{
            break;
        }

    }
    //__mem_free(&sem);
    return 0;
}

sem_t _sem::openSem(unsigned int init) {
    sem_t sem= (_sem*)MemoryAllocator::mem_alloc(sizeof(_sem));
    sem->value=(int)init;
    sem->opened=1;
    sem->waitlist.head= nullptr;
    sem->waitlist.tail= nullptr;
    return sem;
}

