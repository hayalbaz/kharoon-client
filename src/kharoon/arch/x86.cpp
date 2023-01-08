#include "../handler.h"
#include "../context.h"
#include "../utils.h"
#include <libunwind-x86_64.h>

constexpr const char *kharoon_get_register_name(x86_64_regnum_t reg)
{
    switch (reg) {
    case UNW_X86_64_RAX:
        return "RAX";
    case UNW_X86_64_RDX:
        return "RDX";
    case UNW_X86_64_RCX:
        return "RCX";
    case UNW_X86_64_RBX:
        return "RBX";
    case UNW_X86_64_RSI:
        return "RSI";
    case UNW_X86_64_RDI:
        return "RDI";
    case UNW_X86_64_RBP:
        return "RBP";
    case UNW_X86_64_RSP:
        return "RSP";
    case UNW_X86_64_R8:
        return "R8";
    case UNW_X86_64_R9:
        return "R9";
    case UNW_X86_64_R10:
        return "R10";
    case UNW_X86_64_R11:
        return "R11";
    case UNW_X86_64_R12:
        return "R12";
    case UNW_X86_64_R13:
        return "R13";
    case UNW_X86_64_R14:
        return "R14";
    case UNW_X86_64_R15:
        return "R15";
    case UNW_X86_64_RIP:
        return "RIP";
    default:
        return "???";
        break;
    }
}

void kharoon_dump_registers(int fd, unw_cursor_t *cursor, unw_context_t *uc)
{
    using namespace kharoon;
    unw_word_t reg;
    for (int i = UNW_X86_64_RAX; i <= UNW_X86_64_RIP; ++i) {
        UNW_CALL(unw_get_reg(cursor, static_cast<x86_64_regnum_t>(i), &reg));
        context::get()->writeTo(fd, &reg, sizeof(reg));
        context::get()->writeTo(fd, KHAROON_ESCAPE);
    }
}
