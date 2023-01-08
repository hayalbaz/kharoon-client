#ifndef CONTEXT_H
#define CONTEXT_H

#include <vector>
#include <string>
#include <string_view>
#include <signal.h>
#include <atomic>
#include <unordered_map>

extern volatile sig_atomic_t fatal_error_in_progress;
extern volatile sig_atomic_t init_in_progress;

namespace kharoon
{
    // Used to hold all relevant information to be used in an event of dump
    class context
    {
    public:
        static context *get();
        void add_new_object(std::string_view objectPath);
        bool add_metadata(std::string_view key, const void *metadata, std::size_t sz);
        void writeTo(int fd, const char *str) const;
        void writeTo(int fd, const void *buf, std::size_t len) const;
        void disable_crash_handler();
        void setup_crash_handler();
        void set_dump_system_environment(bool dump_system_environment);
        void set_dump_hardware_information(bool dump_hardware_information);
        void set_restart_on_crash(bool restart_on_crash);

    public:
        static constexpr std::size_t PROC_NAME_LENGTH = 100;
        static constexpr auto KHAROON_SERVER_NAME = "kharoon-server";

    private:
        static void handler(int signum);
        context();
        void initialize_signal_handlers(void (*handler)(int));
        void dump_unwind();
        void dump_shared_libraries();
        void dump_objects();
        void dump_metadata();
        void dump_flags();

    private:
        // Used for holding the procedure name during stack unwinding
        std::string proc_name;
        // Used for holding paths to the objects to be saved when dumping
        std::vector<std::string> objects;
        // Used for holding metada
        std::unordered_map<std::string, std::vector<char>> metadata_map;
        // Used to define which signals we will catch
        std::vector<int> signals;
        // File descriptor to write dump data
        int dump_fd;
        // Path to the pid file of this executable
        std::string pid_file;
        // Flags
        bool dump_system_environment;
        bool dump_hardware_information;
        bool restart_on_crash;
    };
}


#endif // CONTEXT_H
