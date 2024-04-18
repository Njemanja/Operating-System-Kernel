//
// Created by marko on 20.4.22..
//
#include "../javniTestovi_2023_1_1/printing.hpp"

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../lib/mem.h"
#include "../h/sleep.hpp"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
joinList TCB::joinlist;
sleepList TCB::sleeplist;
uint64 TCB::time = 0;
TCB *TCB::main = nullptr;

TCB *TCB::createThread(Body body, Arg arg)
{

    void* stack=mem_alloc(sizeof (uint64)*DEFAULT_STACK_SIZE);
    TCB* thread= new TCB(body, arg, stack);

    if(!body){
        running=thread;
        main=thread;
    }else{
        Scheduler::put(thread);
    }

    return  thread;
}

TCB *TCB::makeThread(TCB::Body body, TCB::Arg arg) {
    void* stack=mem_alloc(sizeof (uint64)*DEFAULT_STACK_SIZE);
    TCB* thread=new TCB(body, arg,stack);
    return  thread;
}

void TCB::start() {
    Scheduler::put(this);
}

void TCB::yield()
{
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();
    if(running== nullptr){
        running=main;
    }
    /*if(TCB::f){
        running=main;
    }*/
    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::semDisp() {
    TCB *old = running;
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    exit();
}



int TCB::exit() {
    TCB* running=TCB::running;
    if(running->isFinished()){
        return -1;
    }
    //joinlist.release(running);
    running->setFinished(true);
    thread_dispatch();
    return 0;
}

void TCB::deleteThread() {

    TCB* tmp=Scheduler::get();

    if(tmp){
        int id=tmp->idThread;
        Scheduler::put(tmp);

        while (true){
            tmp=Scheduler::get();
            if(tmp->idThread==id){
                break;
            }
            if(!(tmp->idThread==this->idThread)){
                Scheduler::put(tmp);
            }
        }

    }

}




int TCB::join(TCB *thread) {
    //TCB* old= TCB::running;

    while (!thread->isFinished()){
        //thread_dispatch();
		TCB::dispatch();
    }
    //joinlist.add(thread, old);
    //running=Scheduler::get();
    //TCB::contextSwitch(&old->context, &running->context);

     return 0;
}


int TCB::sleep(time_t time) {
    /*releaseSleep();
    uint64 now=TCB::time;
    TCB *old = running;
    sleeplist.add(old, time, now);
    running = Scheduler::get();
    while(running->idThread==old->idThread){
        running=Scheduler::get();
    }
    TCB::contextSwitch(&old->context, &running->context);*/
    return 0;
}

void TCB::releaseSleep() {
    uint64 now=TCB::time;
    sleeplist.release(now);
}



