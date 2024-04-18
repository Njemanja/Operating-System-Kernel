//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/mem.h"
#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/joinList.hpp"
#include "sleep.hpp"
// Thread Control Block
//#include "syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"

static int id=0;

class TCB
{
public:
    ~TCB() { delete[] stack; }
    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);
    using Arg = void (*);

    static TCB *createThread(Body body, Arg arg);

    static void yield();

    static TCB *running;
    static TCB *main;
    static int sleep(time_t time);
    static void releaseSleep();

    static int exit();
    static joinList joinlist;
    static sleepList sleeplist;

    static int join(TCB* thread);
    void start();
    static TCB *makeThread(Body body, Arg arg);

    void *operator new(size_t size) {
        return MemoryAllocator::mem_alloc(size);;
    }
    void operator delete(void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }
        //    MemoryAllocator::mem_free(c); __mem_free(ptr);



    void deleteThread();
private:
    friend class _sem;



    TCB(Body body, Arg arg,void* st) :
            body(body),
            arg(arg),
            stack(body != nullptr ? (uint64*)st : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64)  &stack[STACK_SIZE] : 0
                    }),
            timeSlice(DEFAULT_TIME_SLICE),
            finished(false), idThread(id++)
    {
        //if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    Arg arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    int idThread;
    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();
    static void semDisp();

    static uint64 timeSliceCounter;
    static uint64 time;

    static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
