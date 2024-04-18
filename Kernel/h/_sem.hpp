
#ifndef __sem_hpp
#define __sem_hpp

#include "tcb.hpp"
#include "list.hpp"
#include "syscall_c.hpp"
#include "waitList.hpp"



class _sem{

public:



    //void *operator new(size_t size) { return __mem_alloc(size); }    //MemoryAllocator::mem_free(c);
    //void operator delete(void *ptr) { __mem_free(ptr); }

    void *operator new(size_t size) {
        return MemoryAllocator::mem_alloc(size);;
    }
    void operator delete(void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }
    static int signal(sem_t sem);
    static int wait(sem_t sem);
    static sem_t openSem(unsigned init);
    static int close(sem_t sem);
    friend class TCB;


    waitList waitlist;

private:

    int value;
    //static waitList waitList;

    int opened;

};




#endif //__sem_hpp
