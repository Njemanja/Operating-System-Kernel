

#include "../h/tcb.hpp"


bool sleepList::empty() {
    if(head){
        return false;
    }
    return true;
}

TCB* sleepList::get() {
    if(empty()){
        return nullptr;
    }
    TCB* first= head->thread;
    if(head->next){
        head=head->next;
    }else{
        head= nullptr;
        tail= nullptr;
    }
    return first;
}

void sleepList::add(TCB* thread, time_t time, uint64 now) {
    /*ThreadSleepELement* node= (ThreadSleepELement*)__mem_alloc(sizeof (ThreadSleepELement));

    node->thread=thread;
    node->sleepTime=time+now;
    node->next= nullptr;
    if(head){
        //node->sleepTime+=now;
        tail->next=node;
        tail=tail->next;
    }else{
        //node->sleepTime+=now;
        head=node;
        tail=head;
    }*/
}

void sleepList::release(uint64 now) {

    while (true){
        if(!head){
            break;
        }
        if(head->sleepTime<=now){
            TCB* node=get();
            Scheduler::put(node);
        }else{
            break;
        }
    }


    /*ThreadJoinELement* tmp=head;
    ThreadJoinELement* prev= nullptr;
    while(tmp){
        if(tmp->threadToWait==running){
            Scheduler::put(tmp->thread);
            if(prev){
                prev->next=tmp->next;
            }else{
                head=tmp->next;
            }
            if(tmp==tail){
                tail=prev;
            }
        }
        prev=tmp;
        tmp=tmp->next;
    }*/
}