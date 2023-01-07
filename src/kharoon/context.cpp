#include "context.h"
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include "handler.h"

namespace kharoon
{
    context::context()
        : proc_name(std::string(PROC_NAME_LENGTH, '\0')),
          signals({SIGSEGV, SIGILL, SIGFPE, SIGABRT})
    {
        setup_crash_handler();
    }

    void context::setup_crash_handler()
    {
        struct sigaction new_action;
        new_action.sa_handler = kharoon_handler;
        sigemptyset(&new_action.sa_mask);
        new_action.sa_flags = 0;
        initialize_signal_handlers(&new_action);
    }

    void context::initialize_signal_handlers(void *action)
    {
        auto *action_ptr = reinterpret_cast<struct sigaction*>(action);
        for (auto signal : signals) {
            sigaction(signal, action_ptr, NULL);
        }
    }

    context *context::get()
    {
        static context ctx;
        return &ctx;
    }

    char *context::get_proc_name()
    {
        return proc_name.data();
    }

    void context::add_new_object(std::string_view objectPath)
    {
        objects.emplace_back(objectPath);
    }

    void context::writeTo(int fd, const char *str) const
    {
        context::writeTo(fd, static_cast<const void*>(str), std::strlen(str));
    }

    void context::writeTo(int fd, const void *buf, std::size_t len) const
    {
        write(fd, buf, len);
    }
}
