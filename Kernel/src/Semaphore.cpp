#include "../h/syscall_cpp.hpp"
#include "../javniTestovi_2023_1_1/printing.hpp"

int Semaphore::signal() {
    sem_signal(myHandle);
    return 0;
}

int Semaphore::wait() {
    sem_wait(myHandle);
    return 0;
}

Semaphore::Semaphore(unsigned int init) {
    myHandle= nullptr;
    sem_open(&myHandle, init);

}

Semaphore::~Semaphore() {
    //printString("Semaphore deleted!\n");
    sem_close(myHandle);
}

