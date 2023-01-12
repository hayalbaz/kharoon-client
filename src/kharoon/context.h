#ifndef KHAROON_CONTEXT_H
#define KHAROON_CONTEXT_H

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
    class compatibility;
    /**
     * @brief The context class business logic of the library is implemented by this class.
     */
    class context
    {
    public:
        static context *get();
        void add_new_object(std::string_view objectPath);
        bool add_metadata(std::string_view key, const void *metadata, std::size_t sz);
        void disable_crash_handler();
        void setup_crash_handler();
        void set_dump_system_environment(bool dump_system_environment);
        void set_dump_hardware_information(bool dump_hardware_information);
        void set_restart_on_crash(bool restart_on_crash, std::string_view executable_path);
        int add_command_line_argument(std::string_view arg);
        void set_path_to_server(std::string_view path_to_server);

        void writeTo(int fd, const char *str) const;
        void writeTo(int fd, const void *buf, std::size_t len) const;
        void writeLn(int fd, const char *str) const;
        void writeLn(int fd, const void *buf, std::size_t len) const;

    public:
        static constexpr std::size_t PROC_NAME_LENGTH = 100;
        static constexpr auto KHAROON_SERVER_NAME = "kharoon-server";

    private:
        static void handler(int signum);
        context();
        void initialize_signal_handlers(void (*handler)(int));
        void dump_unwind();
        void dump_stack_frame(const char *start, const char *end);
        void dump_shared_libraries();
        void dump_objects();
        void dump_metadata();
        void dump_flags();
        void dump_system_information();
        void dump(const char *str) const;
        void dump(const void *buf, std::size_t len) const;
        void dumpLn(const char *str) const;
        void dumpLn(const void *buf, std::size_t len) const;

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
        // Path to the self executable
        std::string executable_path;
        // Path to the kharoon-server executable
        std::string server_path;
        // Argument vector that will be passed to the restarted executable
        std::vector<char*> argv;
        // Flags
        bool dump_system_environment;
        bool dump_hardware_information;
        bool restart_on_crash;
        compatibility *compatibility_layer;
        static constexpr std::size_t PIPE_FD_STR_SIZE = 10;
    };


}


#endif // KHAROON_CONTEXT_H
