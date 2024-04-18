#include "../h/tcb.hpp"

#include "../javniTestovi_2023_1_1/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_cpp.hpp"
#include "../javniTestovi_2023_1_1/Threads_C_API_test.hpp"
#include "../javniTestovi_2023_1_1/Threads_CPP_API_test.hpp"
#include "../javniTestovi_2023_1_1/ConsumerProducer_CPP_Sync_API_test.hpp"
#include "../javniTestovi_2023_1_1/ConsumerProducer_C_API_test.hpp"
#include "../javniTestovi_2023_1_1/ThreadSleep_C_API_test.hpp"
#include "../javniTestovi_2023_1_1/ConsumerProducer_CPP_API_test.hpp"
#include "../javniTestovi_2023_1_1/System_Mode_test.hpp"
//#include "../h/MemoryAllocator.hpp"
#include "../h/workers.hpp"
//#include "../h/m.hpp"





extern void userMain();
int finish=0;
void wrp(void* arg){
    userMain();
    finish=1;
}




int main(){

    MemoryAllocator::initMemoryAllocator();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap|1);
    //Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    TCB::running  = TCB::createThread(nullptr, nullptr);
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    //userMain();

    thread_t u;
    thread_create(&u, wrp, nullptr);
    while (finish==0) {
       thread_dispatch();
    }

    return 0;
}
