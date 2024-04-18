

#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp"


bool joinList::empty() {
    if(head){
        return false;
    }
    return true;
}

TCB* joinList::get() {
    if(empty()){
        return nullptr;
    }
    TCB* first= head->thread;
    /*if(head->next){
        head=head->next;
    }*/
    head=head->next;
    return first;
}

void joinList::add(TCB* thread, TCB* myThread) {
    ThreadJoinELement* node= (ThreadJoinELement*)mem_alloc(sizeof (ThreadJoinELement));
    node->threadToWait=thread;
    node->thread=myThread;
    node->next= nullptr;
    if(head){
        tail->next=node;
        tail=tail->next;
    }else{
        head=node;
        tail=head;
    }
}

void joinList::release(TCB *running) {
    ThreadJoinELement* tmp=head;
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

    }
}