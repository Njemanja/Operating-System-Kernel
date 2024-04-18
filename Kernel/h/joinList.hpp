#include "tcb.hpp"
#include "../lib/mem.h"
class joinList
{
private:
    struct ThreadJoinELement{
        TCB* threadToWait;
        TCB* thread;
        ThreadJoinELement* next;
    };

public:
    ThreadJoinELement *head= nullptr;
    ThreadJoinELement *tail= nullptr;

    bool empty();
    TCB* get();
    void add(TCB* thread, TCB* myThread);
    void release(TCB *running);
};


