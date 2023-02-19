#ifndef KHAROON_CLIENT_INCLUDED
#define KHAROON_CLIENT_INCLUDED

#include <cstddef>
#if defined(_MSC_VER)
    #define KHAROON_EXPORT __declspec(dllexport)
    #define KHAROON_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    #define KHAROON_EXPORT __attribute__((visibility("default")))
    #define KHAROON_IMPORT
#else
    //  do nothing and hope for the best?
    #define KHAROON_EXPORT
    #define KHAROON_IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

/**
 * @brief kharoon_client_init starts the initialization of kharoon, you need to call this before calling any other
 * function. Between this and kharoon_client_init_end() function call no signals will be cought. Similarly, outside
 * of this interval, calls to any kharoon_* functions will be ignored.
 * @param path_to_server absolute path to the kharoon server, if empty string given kharoon will dump to stdout
 * @return 0 on success, any other number indicates error.
 * @see kharoon_client_init_end
 * @pre it is assumed that path_to_server is a valid path and not null.
 */
KHAROON_EXPORT int kharoon_client_init_start(const char *path_to_server = "");
/**
 * @brief kharoon_client_init completes the initialization of kharoon, you need to call this to enable handling crashes.
 * @return 0 on success, any other number indicates error.
 * @see kharoon_client_init_start
 */
KHAROON_EXPORT int kharoon_client_init_end();
/**
 * @brief kharoon_set_dump_path sets the path that will be used to save the cump files. If it is not called, it will
 * dump to the default dump directory set by the kharoon-server.
 * @param dump_dir path that will be used to create the dump file. If folders do not exist, they will be created.
 * @pre it is assumed that dump_dir is a file path. kharoon-server will ignore the request to save dump if it not a
 * path
 */
KHAROON_EXPORT void kharoon_set_dump_path(const char *dump_dir);
/**
 * @brief kharoon_add_object_to_dump adds a path to an object to be included in the final dump. Given object will be
 * bundled with the dump file.
 * @param obj_path path to a file.
 * @pre it is assumed that the obj_path points to a valid file or folder, and it is of reasonable size. kharoon-server
 * will filter out object that it cannot find.
 */
KHAROON_EXPORT void kharoon_add_object_to_dump(const char *obj_path);
/**
 * @brief kharoon_add_metadata_to_dump adds metadata to be included in the final dump. It works as a key-value store,
 * which can be recovered later from the dump.
 * @param key that will be used to retrieve the metada, cannot contain ','.
 * @param metadata the metadata that will be saved.
 * @return 0 if successfull, non-zero if key contains ',' or init_start is not called.
 */
KHAROON_EXPORT int kharoon_add_metadata_to_dump(const char *key, const void *metadata, size_t sz);
/**
 * @brief kharoon_set_restart_after_crash when a crash happens and kharoon creates a dump, it will restart the caller
 * program again.
 */
KHAROON_EXPORT void kharoon_set_restart_after_crash(const char *executable_path);
/**
 * @brief kharoon_reset_restart_after_crash disables restarting after a crash, it is the default setting.
 */
KHAROON_EXPORT void kharoon_reset_restart_after_crash();
/**
 * @brief kharoon_set_dump_system_environment includes information(cpu usage, ram usage) about other running processes
 * in the final dump.
 */
KHAROON_EXPORT void kharoon_set_dump_system_environment();
/**
 * @brief kharoon_reset_dump_system_environment disables including information about other running proccesses in the
 * final dump, it is the default setting.
 */
KHAROON_EXPORT void kharoon_reset_dump_system_environment();
/**
 * @brief kharoon_set_dump_peripherals includes information about hardware the software is running on. This information
 * includes: CPU, GPU, GPU driver name and version, RAM, connected USB devices, displays.
 */
KHAROON_EXPORT void kharoon_set_dump_hardware_information();
/**
 * @brief kharoon_reset_dump_hardware_information disables including information about hardware the software is running
 * on, it is the default setting.
 */
KHAROON_EXPORT void kharoon_reset_dump_hardware_information();
/**
 * @brief kharoon_add_command_line_argument adds an argument to the argv vector that will be passed to the restarting
 * process, if restarting after crash is enabled. The input will be copied to an internal buffer.
 * @param arg argument that will be passed to the restarting process.
 * @return 0 if succesfull, non-zero if restarting is not enabled.
 * @see kharoon_set_restart_after_crash
 * @note the argument vector is not kept across changing the ability to restart. If you set up your arguments but
 * disable them at some point, you will have to add them again for them to be passed to restarting process.
 */
KHAROON_EXPORT int kharoon_add_command_line_argument(const char *arg);
#endif
