


#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
//#include "../lib/hw.h"
#include "../javniTestovi_2023_1_1/printing.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_sem.hpp"
//#include "../h/MemoryAllocator.hpp"




void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    uint64 sepc = r_sepc() + 4;
    uint64 sstatus = r_sstatus();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        size_t code;
        __asm__ volatile ("mv %0, a0" : "=r"(code));
        if(code==0x01){
            uint64 c_int;
            __asm__ volatile ("mv %0, a1" : "=r"(c_int));
            size_t c=(size_t)c_int;
            void* t=MemoryAllocator::mem_alloc(c);
            uint64 value_int=(uint64)t;
            __asm__ volatile("sd a0, 10*8(fp)");
            __asm__ volatile("mv a0, %0" : : "r"(value_int));

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code==0x02){
            uint64 c_int;
            __asm__ volatile ("mv %0, a1" : "=r"(c_int));
            void* c=(void*)c_int;
            MemoryAllocator::mem_free(c);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code==0x42){
            uint64 c_int;
            __asm__ volatile ("mv %0, a1" : "=r"(c_int));
            char c=(char)c_int;
            __putc(c);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code==0x41){
            char c=__getc();
            __asm__ volatile("sd a0, 10*8(fp)");
            __asm__ volatile("mv a0, %0" : : "r"(c));
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if(code==0x31){
            uint64 time_int;
            __asm__ volatile ("mv %0, a1" : "=r"(time_int));
            time_t time=(time_t)time_int;
            TCB::sleep(time);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if (code==0x21){ //SEM OPEN
            uint64 init;
            __asm__ volatile ("mv %0, a1" : "=r"(init));
            uint64 id;
            __asm__ volatile ("mv %0, a2" : "=r"(id));
            sem_t handler=  _sem::openSem((unsigned )init);
            _sem** handler1=(_sem**)id;
            *(handler1)=(handler);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }

        else if (code==0x22){
            uint64 sem_int;
            __asm__ volatile ("mv %0, a1" : "=r"(sem_int));
            _sem* sem=(_sem*)sem_int;
            _sem::close(sem);
            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if (code==0x23){
            uint64 sem_int;
            __asm__ volatile ("mv %0, a1" : "=r"(sem_int));
            _sem* sem=(_sem*)sem_int;
            _sem::wait(sem);
            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if (code==0x24){
            uint64 sem_int;
            __asm__ volatile ("mv %0, a1" : "=r"(sem_int));
            _sem* sem=(_sem*)sem_int;
            _sem::signal(sem);
            w_sstatus(sstatus);
            w_sepc(sepc);

        }
        else if (code==0x11){

            uint64 handle_int;
            __asm__ volatile ("mv %0, a1" : "=r"(handle_int));
            uint64 int_body;
            uint64 int_arg;
            __asm__ volatile ("mv %0, a2" : "=r"(int_body));
            __asm__ volatile ("mv %0, a7" : "=r"(int_arg));
            TCB::Body body= (TCB::Body)int_body;
            TCB::Arg arg= (TCB::Arg)int_arg;

            uint64 makeCreate;
            __asm__ volatile ("mv %0, a6" : "=r"(makeCreate));


            thread_t* h=(thread_t*)handle_int;
            thread_t t= nullptr;
            if(makeCreate){
                t = TCB::createThread(body, arg);
            }else{
                t = TCB::makeThread(body, arg);
            }

            *(h)=t;
            handle_int=(uint64)h;
            __asm__ volatile("sd a0, 10*8(fp)");
            __asm__ volatile("mv a0, %0" : : "r"(handle_int));

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if (code==0x12){
            int value=TCB::exit();
            uint64 value_int=(uint64)value;
            __asm__ volatile("sd a0, 10*8(fp)");
            __asm__ volatile("mv a0, %0" : : "r"(value_int));
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if (code==0x13){
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if (code==0x15){
            uint64 thread_int;
            __asm__ volatile ("mv %0, a1" : "=r"(thread_int));
            TCB* thread=(TCB*)thread_int;
            thread->start();

            w_sstatus(sstatus);
            w_sepc(sepc);
        }

        else if (code==0x16){
            uint64 thread_int;
            __asm__ volatile ("mv %0, a2" : "=r"(thread_int));
            thread_t * thread=(thread_t*)thread_int;

            TCB* t=(TCB*)(*thread);
            t->deleteThread();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        else if (code==0x14){
            uint64 thread_int;
            __asm__ volatile ("mv %0, a1" : "=r"(thread_int));
            TCB* handle=(TCB*)(thread_int);
            TCB::join(handle);
            w_sstatus(sstatus);
            w_sepc(sepc);
        }

        else{
            // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }


    }
    /*else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        //TCB::timeSliceCounter++;
        //TCB::time++;
        //TCB::releaseSleep();

       // if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        //{
           // uint64 volatile sepc = r_sepc();
            //uint64 volatile sstatus = r_sstatus();
           // TCB::timeSliceCounter = 0;
            //TCB::dispatch();
            //w_sstatus(sstatus);
         //   w_sepc(sepc);
        //}
    //}
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }*/
    else
    {

        printString("\nUNEXPECTED TRAP CAUSE!\n");
        printString("Scause: ");
        printInt(scause);
        printString("\n");
        printString("Spec: ");
        printInt(r_sepc());
        printString("\n");
        while(1){}

        // unexpected trap cause
    }



}

void Riscv::handleSupervisorTrapC() {
    console_handler();
}

void Riscv::handleSupervisorTrapT() {
    mc_sip(SIP_SSIP);

}


/*.extern _ZN5Riscv20handleSupervisorTrapEv

.align 4
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function
_ZN5Riscv14supervisorTrapEv:
    j syscallroutine
    sret #j timerroutine
    sret
    sret
    sret
    sret
    sret
    sret
    sret
    sret #j consoleroutine
    syscallroutine:
    	# push all registers to stack
        addi sp, sp, -256
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        sd x\index, \index * 8(sp)
        .endr

        call _ZN5Riscv20handleSupervisorTrapEv

        # pop all registers from stack
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, \index * 8(sp)
        .endr
        addi sp, sp, 256
        sret
*/