#ifndef KHAROON_UTILS_H
#define KHAROON_UTILS_H

#include "context.h"
#define UNW_LOCAL_ONLY
#include <libunwind.h>

#if defined(_MSC_VER)
    #define KHAROON_DEBUG _DEBUG
#elif defined(__GNUC__)
    #define KHAROON_DEBUG NDEBUG
#else
    #define KHAROON_DEBUG
#endif

#ifdef KHAROON_DEBUG
    #define UNW_CALL(fn) do { \
                             auto ec = fn; \
                             if (ec != 0) kharoon::util::print_unwind_error(ec, __FILE__, __LINE__); \
                         } while (0)
#else
    #define UNW_CALL(fn) fn
#endif

#if defined(_MSC_VER)
    static const char *KHAROON_ESCAPE = "\r\n";
#elif defined(__GNUC__)
    static const char *KHAROON_ESCAPE = "\n";
#else
    static const char *KHAROON_ESCAPE = "\n";
#endif

namespace kharoon
{
namespace util
{
    static void print_unwind_error(int ec, const char* fname, int line)
    {
        context::get()->writeTo(2, "libunwind error: ");
        context::get()->writeTo(2, unw_strerror(ec));
        std::abort();
    }
}
}

#endif // KHAROON_UTILS_H
