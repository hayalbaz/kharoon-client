#ifndef CONTEXT_H
#define CONTEXT_H

#include <vector>
#include <string>
#include <string_view>
#include <signal.h>
#include <atomic>

static volatile sig_atomic_t fatal_error_in_progress = 0;
static volatile sig_atomic_t init_in_progress = 0;

namespace kharoon
{
    // Used to hold all relevant information to be used in an event of dump
    class context
    {
    public:
        static context *get();
        char *get_proc_name();
        void add_new_object(std::string_view objectPath);
        void writeTo(int fd, const char *str) const;
        void writeTo(int fd, const void *buf, std::size_t len) const;
        void disable_crash_handler();
        void setup_crash_handler();


    public:
        static constexpr std::size_t PROC_NAME_LENGTH = 100;
        static constexpr auto KHAROON_SERVER_NAME = "kharoon-server";

    private:
        static void handler(int signum);
        context();
        void initialize_signal_handlers(void (*handler)(int));
        void dump_unwind();
        void dump_shared_libraries();
        void dump_system_information();

    private:
        // Used for holding the procedure name during stack unwinding
        std::string proc_name;
        // Used for holding paths to the objects to be saved when dumping
        std::vector<std::string> objects;
        // Used to define which signals we will catch
        std::vector<int> signals;
        // File descriptor to write dump data
        int fd;
    };
}


#endif // CONTEXT_H
