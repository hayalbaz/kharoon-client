#include <iostream>
#include <kharoon/client/kharoon.h>

void crash_unhandled_exception()
{
    throw std::runtime_error("test");
}

void crash_nullptr_deref()
{
    int *i = nullptr;
    std::cout << *i;
}

void crash_abort()
{
    std::abort();
}

void crash_arithmatic_error()
{
    int i = 1/0;
    std::cout << i;
}

void testa()
{
    std::cout << "a" << std::endl;

    kharoon_client_dump_unwind_stdout();
    //crash_nullptr_deref();
    //crash_abort();
    //crash_arithmatic_error();
    //crash_unhandled_exception();
}

void testb()
{
    std::cout << "b" << std::endl;
    testa();
}

void testc()
{
    std::cout << "c" << std::endl;
    testb();
}

int main()
{
    kharoon_client_init();
    testc();
    return 0;
}
