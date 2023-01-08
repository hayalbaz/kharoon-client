#include "../context.h"
#include "../utils.h"
#include <libunwind-x86_64.h>

void kharoon_dump_registers(int fd, unw_cursor_t *cursor, unw_context_t *uc)
{
    using namespace kharoon;
    unw_word_t val;
    for (int i = UNW_X86_64_RAX; i <= UNW_X86_64_RIP; ++i) {
        auto reg = static_cast<x86_64_regnum_t>(i);
        UNW_CALL(unw_get_reg(cursor, reg, &val));
        context::get()->writeTo(fd, unw_regname(reg));
        context::get()->writeTo(fd, " : ");
        context::get()->writeTo(fd, &val, sizeof(val));
        context::get()->writeTo(fd, KHAROON_ESCAPE);
    }
}
