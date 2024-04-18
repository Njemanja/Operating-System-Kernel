#include "../h/syscall_cpp.hpp"


void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}


