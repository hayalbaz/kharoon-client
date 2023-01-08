#ifndef KHAROON_CLIENT_INCLUDED
#define KHAROON_CLIENT_INCLUDED

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
 * @brief kharoon_client_init initializes kharoon, you need to call this before calling any other function
 * @param dump_dir path that will be used to create the dump file. If folders do not exist, they will be created.
 * @return 0 on success, any other number indicates error.
 */
KHAROON_EXPORT int kharoon_client_init(const char *dump_dir);
/**
 * @brief kharoon_client_dump_unwind_stdout dumps the current stack trace to stdout.
 */
KHAROON_EXPORT void kharoon_client_dump_unwind_stdout();
/**
 * @brief kharoon_client_dump_unwind_stdout dumps the currently loaded shared libraries to stdout.
 */
KHAROON_EXPORT void kharoon_client_dump_shared_libraries_stdout();
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
 * @param key that will be used to retrieve the metada.
 * @param metadata the metadata that will be saved.
 */
KHAROON_EXPORT void kharoon_add_metadata_to_dump(const char *key, const void *metadata);
/**
 * @brief kharoon_set_restart_after_crash when a crash happens and kharoon creates a dump, it will restart the caller
 * program again.
 */
KHAROON_EXPORT void kharoon_set_restart_after_crash();
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
 * includes: CPU, GPU, GPU Driver, RAM, connected USB devices, displays.
 */
KHAROON_EXPORT void kharoon_set_dump_hardware_information();
/**
 * @brief kharoon_reset_dump_hardware_information disables including information about hardware the software is running
 * on, it is the default setting.
 */
KHAROON_EXPORT void kharoon_reset_dump_hardware_information();

#endif
