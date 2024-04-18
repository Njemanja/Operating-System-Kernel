#include "../h/syscall_cpp.hpp"
#include "../javniTestovi_2023_1_1/printing.hpp"
#include "../h/scheduler.hpp"

void Thread::join() {
    thread_join(this->myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle= nullptr;
    //thread_create(&myHandle, body, arg);
    thread_make(&myHandle, body, arg);


}

int Thread::start() {
    thread_start(myHandle);
    return 0;
}


Thread::~Thread()

noexcept {
    thread_delete(&myHandle);
    //thread_exit();----------------------------------------------------------------------------------
}

void Thread::wrap(void *arg) {
    Thread* t=(Thread*)arg;
    t->run();
}

Thread::Thread() :   Thread(wrap, this) {
    //printString("Thread()\n");

}

int Thread::sleep(time_t time) {
    time_sleep(time);
    return 0;
}