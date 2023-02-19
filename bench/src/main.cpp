#include <cstring>
#include <iostream>
#include <kharoon/client/kharoon.h>
#include <thread>
using namespace std::chrono_literals;

#if defined(_MSC_VER)
    #define KHAROON_EXPORT __declspec(noinline)
#elif defined(__GNUC__)
    #define KHAROON_NOINLINE __attribute__((noinline))
#else
    //  do nothing and hope for the best?
    #define KHAROON_EXPORT
    #pragma warning Unknown noinline semantics.
#endif

#define KHAROON_PREVENT_INLINE asm (""); static int var = 0;

void KHAROON_NOINLINE crash_unhandled_exception()
{
    KHAROON_PREVENT_INLINE;
    throw std::runtime_error("test");
}

void KHAROON_NOINLINE crash_nullptr_deref()
{
    KHAROON_PREVENT_INLINE;
    int *leak = new int[1000000]; // leak about 4MB each time to check if restarting the process carries the leaks
    int *i = nullptr;
    std::cout << *i;
}

void KHAROON_NOINLINE crash_abort()
{
    KHAROON_PREVENT_INLINE;
    std::abort();
}

void KHAROON_NOINLINE crash_arithmatic_error()
{
    KHAROON_PREVENT_INLINE;
    int i = 1/0;
    std::cout << i;
}

void KHAROON_NOINLINE testa(int input)
{
    KHAROON_PREVENT_INLINE;
    //BUG: in release mode uncommenting this causes test functions to get inlined, and only shows main function at the
    //top of the callstack.
    //kharoon_client_dump_unwind_stdout();
    crash_nullptr_deref();
    //crash_abort();
    //crash_arithmatic_error();
    //crash_unhandled_exception();
}

void KHAROON_NOINLINE testb()
{
    KHAROON_PREVENT_INLINE;
    char test[5] = "test";
    testa(3);
}

void KHAROON_NOINLINE testc()
{
    KHAROON_PREVENT_INLINE;
    for (int i = 0; i < 2000; ++i) {
        std::this_thread::sleep_for(1ms);
    }
    testb();
}

int main(int argc, char *argv[])
{
    std::string path;
    if (argc < 2) {
        path = "";
    }
    else {
        path = argv[1];
    }
    kharoon_client_init_start(path.c_str());
    kharoon_add_object_to_dump("/path/to/object");
    std::string metadata("value");
    kharoon_add_metadata_to_dump("key", metadata.c_str(), metadata.size());
    kharoon_set_dump_hardware_information();
    kharoon_set_dump_system_environment();
    kharoon_set_restart_after_crash(argv[0]);
    kharoon_add_command_line_argument("--restart-me");
    kharoon_client_init_end();

    testc();

    return 0;
}
