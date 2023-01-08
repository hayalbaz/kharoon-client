#include "context.h"
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include "utils.h"

/**
 * @brief kharoon_dump_registers dumps the registers in the current stack frame, it is platform specific and should be
 * linked to correct platform's implementation for it to work.
 * @param fd file descriptor to dump the register names and values
 * @param cursor current frame's cursor
 * @param uc current context
 */
void kharoon_dump_registers(int fd, unw_cursor_t *cursor, unw_context_t *uc);

namespace kharoon
{
    context::context()
        : proc_name(std::string(PROC_NAME_LENGTH, '\0')),
          signals({SIGSEGV, SIGILL, SIGFPE, SIGABRT})
    {}

    void context::disable_crash_handler()
    {
        initialize_signal_handlers(SIG_DFL);
    }

    void context::setup_crash_handler()
    {
        initialize_signal_handlers(&context::handler);
    }

    void context::initialize_signal_handlers(void (*handler)(int))
    {
        struct sigaction new_action;
        new_action.sa_handler = handler;
        sigemptyset(&new_action.sa_mask);
        new_action.sa_flags = 0;
        for (auto signal : signals) {
            sigaction(signal, &new_action, NULL);
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
        writeTo(fd, static_cast<const void*>(str), std::strlen(str));
    }

    void context::writeTo(int fd, const void *buf, std::size_t len) const
    {
        write(fd, buf, len);
    }

    void context::handler(int signum)
    {
        if (fatal_error_in_progress) {
            raise(signum);
        }
        fatal_error_in_progress = 1;

        get()->dump_unwind();
        signal(signum, SIG_DFL);
        raise(signum);
    }

    void context::dump_unwind()
    {
        unw_cursor_t cursor;
        unw_context_t uc;
        unw_word_t ip;
        unw_word_t sp;
        unw_word_t ofs;

        UNW_CALL(unw_getcontext(&uc));

        if (unw_is_signal_frame(&cursor) > 0) {
            UNW_CALL(unw_init_local2(&cursor, &uc, UNW_INIT_SIGNAL_FRAME));
        }
        else {
            UNW_CALL(unw_init_local(&cursor, &uc));
        }

        while (unw_step(&cursor) > 0) {
            std::memset(get_proc_name(), 0, PROC_NAME_LENGTH);
            UNW_CALL(unw_get_proc_name(&cursor, get_proc_name(), PROC_NAME_LENGTH, &ofs));
            writeTo(fd, "[");
            writeTo(fd, get_proc_name());
            writeTo(fd, "] : ");
            writeTo(fd, KHAROON_ESCAPE);
            kharoon_dump_registers(fd, &cursor, &uc);
        }
    }
}
