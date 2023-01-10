#ifndef KHAROON_UTILS_H
#define KHAROON_UTILS_H

#include "context.h"
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <cmath>

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
    /**
     * @brief print_unwind_error this function is called whenever an error in unwind library occurs, it simply aborts
     * the probram since at this point we cannot really recover. It prints the error to stderr.
     * @param ec
     * @param fname
     * @param line
     */
    static void print_unwind_error(int ec, const char* fname, int line)
    {
        context::get()->writeTo(2, "libunwind error: ");
        context::get()->writeTo(2, unw_strerror(ec));
        std::abort();
    }

    template<typename T>
    int get_number_of_digits(T t)
    {
        return t > 0 ? (int) log10 ((double) t) + 1 : 1;
    }

    /**
     * @brief number_to_str_base_10 a naive number to string conversion function for base 10, meant to be signal safe.
     * @param str pre allocated char array.
     * @param t number we want to convert to string.
     * @pre it assumes that str is allocated and has enought space, it should have atleast
     * get_number_of_digits(num) + 1 bytes.
     * @pre it assumes T is a positive integer or zero.
     */
    template<typename T>
    static void number_to_str_base_10(char *str, T number)
    {
        auto digits = get_number_of_digits(number);
        for (int i = 0; i < digits + 1; ++i) {
            if (i == 0) {
                str[digits] = '\0';
                continue;
            }
            auto d = number % 10;
            str[digits - i] = (d & 0b00001111) | 0b00110000;
            number = static_cast<T>(static_cast<double>(number)/10);
        }
    }
}
}

#endif // KHAROON_UTILS_H
