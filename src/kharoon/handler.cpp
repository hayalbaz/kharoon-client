#include "handler.h"
#include "context.h"
#include "utils.h"
#include <cstring>
#include <signal.h>

volatile sig_atomic_t fatal_error_in_progress = 0;

void kharoon_dump_unwind(int fd)
{
    using namespace kharoon;
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_word_t sp;
    unw_word_t ofs;

    UNW_CALL(unw_getcontext(&uc));
    UNW_CALL(unw_init_local(&cursor, &uc));

    while (unw_step(&cursor) > 0) {
        std::memset(context::get()->get_proc_name(), 0, kharoon::context::PROC_NAME_LENGTH);
        UNW_CALL(unw_get_reg(&cursor, UNW_REG_IP, &ip));
        UNW_CALL(unw_get_reg(&cursor, UNW_REG_SP, &sp));
        UNW_CALL(unw_get_proc_name(&cursor, context::get()->get_proc_name(), kharoon::context::PROC_NAME_LENGTH, &ofs));
        context::get()->writeTo(fd, "[");
        context::get()->writeTo(fd, context::get()->get_proc_name());
        context::get()->writeTo(fd, "], ip = ");
        context::get()->writeTo(fd, &ip, sizeof(ip));
        context::get()->writeTo(fd, ", sp = ");
        context::get()->writeTo(fd, &sp, sizeof(sp));
        context::get()->writeTo(fd,  ", ofs = ");
        context::get()->writeTo(fd, &ofs, sizeof(ofs));
        context::get()->writeTo(fd, "\n");
    }
}

void kharoon_handler(int signum)
{
    if (fatal_error_in_progress) {
        raise(signum);
    }
    fatal_error_in_progress = 1;

    kharoon_dump_unwind(1);
    signal (signum, SIG_DFL);
    raise (signum);
}
