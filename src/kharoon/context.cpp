#include "context.h"
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include "utils.h"
#include <fcntl.h>
#include <algorithm>
#include <cassert>
volatile sig_atomic_t fatal_error_in_progress = 0;
volatile sig_atomic_t init_in_progress = 0;

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
    {
#if defined(_MSC_VER)
    #pragma warning Shared library dumping not implemented for Windows yet.
#elif defined(__GNUC__)
        pid_file = "/proc/" + std::to_string(getpid()) + "/maps";
#else
    #pragma warning Unknown shared library semantics, will not dump shared library information.
#endif
    }

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

    void context::add_new_object(std::string_view objectPath)
    {
        if (!init_in_progress) {
            return;
        }
        objects.emplace_back(objectPath);
    }

    bool context::add_metadata(std::string_view key, const void *metadata, std::size_t sz)
    {
        if (!init_in_progress) {
            return false;
        }
        if (key.find(",") != std::string::npos) {
            return false;
        }

        std::vector<char> metadata_vector;
        metadata_vector.reserve(sz);
        std::copy_n(reinterpret_cast<const char*>(metadata), sz, std::back_inserter(metadata_vector));
        metadata_map.emplace(key, std::move(metadata_vector));
        return true;
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
        get()->dump_shared_libraries();
        get()->dump_objects();
        get()->dump_metadata();
        get()->dump_flags();
        signal(signum, SIG_DFL);
        raise(signum);
    }

    void context::dump_unwind()
    {
        writeTo(dump_fd, "<<<backtrace>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
        unw_cursor_t cursor;
        unw_context_t uc;
        unw_word_t ofs;

        UNW_CALL(unw_getcontext(&uc));

        if (unw_is_signal_frame(&cursor) > 0) {
            UNW_CALL(unw_init_local2(&cursor, &uc, UNW_INIT_SIGNAL_FRAME));
        }
        else {
            UNW_CALL(unw_init_local(&cursor, &uc));
        }

        auto res = unw_step(&cursor);
        while (res > 0) {
            std::memset(proc_name.data(), 0, PROC_NAME_LENGTH);
            UNW_CALL(unw_get_proc_name(&cursor, proc_name.data(), PROC_NAME_LENGTH, &ofs));
            writeTo(dump_fd, "[");
            writeTo(dump_fd, proc_name.data());
            writeTo(dump_fd, ":offset=");
            writeTo(dump_fd, &ofs, sizeof(ofs));
            writeTo(dump_fd, "] : ");
            writeTo(dump_fd, KHAROON_ESCAPE);
            kharoon_dump_registers(dump_fd, &cursor, &uc);

            unw_word_t current_sp;
            unw_word_t next_sp;
            UNW_CALL(unw_get_reg(&cursor, UNW_REG_SP, &current_sp));
            res = unw_step(&cursor);
            if (res > 0) {
                UNW_CALL(unw_get_reg(&cursor, UNW_REG_SP, &next_sp));
                dump_stack_frame(reinterpret_cast<const char*>(current_sp), reinterpret_cast<const char*>(next_sp));
            }
        }
        writeTo(dump_fd, "<<</backtrace>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
    }

    void context::dump_stack_frame(const char *start, const char *end)
    {
        assert(end > start);
        auto len = end - start;

        writeTo(dump_fd, "stack frame : ");
        writeTo(dump_fd, start, len);
        writeTo(dump_fd, KHAROON_ESCAPE);
    }

    void context::dump_shared_libraries()
    {
        writeTo(dump_fd, "<<<shared_libraries>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
#if defined(_MSC_VER)
    #pragma warning Shared library dumping not implemented for Windows yet.
#elif defined(__GNUC__)
        auto fd = open(pid_file.c_str(), O_RDONLY);
        if (fd < 0) {
            return;
        }

        constexpr std::size_t SZ = 128;
        char buf[SZ];
        ssize_t read_bytes = 0;

        do {
            std::memset(buf, 0, SZ);
            read_bytes = read(fd, buf, SZ);
            writeTo(dump_fd, buf, read_bytes);
        } while (read_bytes != 0);
#else
    #pragma warning Unknown shared library semantics, will not dump shared library information.
#endif
        writeTo(dump_fd, "<<</shared_libraries>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
    }

    void context::dump_objects()
    {
        writeTo(dump_fd, "<<<objects>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
        for (const auto &obj : objects) {
            writeTo(dump_fd, obj.c_str());
            writeTo(dump_fd, KHAROON_ESCAPE);
        }
        writeTo(dump_fd, "<<</objects>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
    }

    void context::dump_metadata()
    {
        writeTo(dump_fd, "<<<metadata>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
        for (const auto &[key, val] : metadata_map) {
            writeTo(dump_fd, key.c_str());
            writeTo(dump_fd, ",");
            writeTo(dump_fd, val.data(), val.size());
            writeTo(dump_fd, KHAROON_ESCAPE);
        }
        writeTo(dump_fd, "<<</metadata>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
    }

    void context::dump_flags()
    {
        writeTo(dump_fd, "<<<flags>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);

        writeTo(dump_fd, "dump_system_environment,");
        writeTo(dump_fd, dump_system_environment ? "true" : "false");
        writeTo(dump_fd, KHAROON_ESCAPE);

        writeTo(dump_fd, "dump_hardware_information,");
        writeTo(dump_fd, dump_hardware_information ? "true" : "false");
        writeTo(dump_fd, KHAROON_ESCAPE);

        writeTo(dump_fd, "restart_on_crash,");
        writeTo(dump_fd, restart_on_crash ? "true" : "false");
        writeTo(dump_fd, KHAROON_ESCAPE);

        writeTo(dump_fd, "<<</flags>>>");
        writeTo(dump_fd, KHAROON_ESCAPE);
    }

    void context::set_dump_system_environment(bool dump_system_environment)
    {
        if (!init_in_progress) {
            return;
        }
        this->dump_system_environment = dump_system_environment;
    }

    void context::set_dump_hardware_information(bool dump_hardware_information)
    {
        if (!init_in_progress) {
            return;
        }
        this->dump_hardware_information = dump_hardware_information;
    }

    void context::set_restart_on_crash(bool restart_on_crash)
    {
        if (!init_in_progress) {
            return;
        }
        this->restart_on_crash = restart_on_crash;
    }
}
