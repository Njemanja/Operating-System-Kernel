

#include "../h/_sem.hpp"
#include "../h/syscall_c.hpp"
#include "../javniTestovi_2023_1_1/printing.hpp"
bool waitList::empty() {
    if(head){
        return false;
    }
    return true;
}

TCB* waitList::get() {
    if(empty()){
        return nullptr;
    }
    TCB* first= head->thread;
    waitSemElement* p=head;
    head=head->next;
    MemoryAllocator::mem_free(p);
    return first;
}

void waitList::add(TCB* thread) {
    waitSemElement* node= (waitSemElement*)MemoryAllocator::mem_alloc(sizeof (waitSemElement));
    node->thread=thread;
    //node->sem=sem;
    node->next= nullptr;
    if(head){
        tail->next=node;
        tail=tail->next;
    }else{
        head=node;
        tail=head;
    }
}

/*void waitList::release() {
    waitSemElement* tmp=head;
    waitSemElement* prev= nullptr;
    while(tmp){
        if(tmp->sem==sem){
            Scheduler::put(tmp->thread);
            if(prev){
                prev->next=tmp->next;
            }else{
                head=tmp->next;
            }
            if(tmp==tail){
                tail=prev;
            }
            break;
        }
        prev=tmp;
        tmp=tmp->next;

    }
}*/