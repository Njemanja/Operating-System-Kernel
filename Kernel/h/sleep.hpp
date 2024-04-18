#ifndef SLEEP_HPP
#define SLEEP_HPP

/*struct ThreadSleepELement{
    uint64 sleepTime;
    TCB* thread;
    ThreadSleepELement* next;
};


ThreadSleepELement* headSleep;
ThreadSleepELement* tailSleep;
bool emptySleepList();
TCB* getSleepElement();
void addSleepElement(ThreadSleepELement* element);
time_t passed;
void oneTick();*/


#include "tcb.hpp"
#include "../lib/mem.h"
class sleepList
{
private:
    struct ThreadSleepELement{
        uint64 sleepTime;
        TCB* thread;
        ThreadSleepELement* next;
    };

public:
    ThreadSleepELement *head= nullptr;
    ThreadSleepELement *tail= nullptr;

    bool empty();
    TCB* get();
    void add(TCB* thread, time_t time, uint64 now);
    void release(uint64 now);
};





#endif //SLEEP_HPP