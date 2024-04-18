#include "tcb.hpp"
#include "../lib/mem.h"
#include "_sem.hpp"

class waitList
{
private:
    struct waitSemElement{
        TCB* thread;
        //sem_t* sem;
        waitSemElement* next;
    };

public:
    waitSemElement *head= nullptr;
    waitSemElement *tail= nullptr;

    bool empty();
    TCB* get();
    //void add(TCB* thread, sem_t* sem);
    void add(TCB* thread);
    //void release();
};


