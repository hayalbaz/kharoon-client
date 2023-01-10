#ifndef KHAROON_COMPATIBILITY_H
#define KHAROON_COMPATIBILITY_H

#include <cstddef>
#include <string>

/**
 * @brief The compatibility class is the base class that other operating system compatibility classes inherit from.
 */
namespace kharoon
{
    class compatibility
    {
    public:
        virtual ~compatibility() = default;
        /**
         * @brief dump_shared_libraries dumps some of the shared library information into given buffer.
         * @param buffer the buffer that will contain some shared library information.
         * @param size size of the buffer.
         * @return returns the number of bytes dumped, 0 if there are no more, -1 if there was an error.
         * @pre it is assumed that buffer is allocated.
         * @post it is undefined what happens if another dump_* function is called when one of them hasn't returned
         * 0 or -1. If 0 or -1 is returned no system resources will be leaked.
         */
        virtual int dump_shared_libraries(char *buffer, std::size_t size) = 0;
        /**
         * @brief dump_hardware_information dumps some of the hardware information into given buffer
         * @param buffer the buffer that will contain some hardware related information.
         * @param size size of the buffer.
         * @return returns the number of bytes dumped, 0 if there are no more, -1 if there was an error.
         * @pre it is assumed that buffer is allocated.
         * @post it is undefined what happens if another dump_* function is called when one of them hasn't returned
         * 0 or -1. If 0 or -1 is returned no system resources will be leaked.
         */
        virtual int dump_hardware_information(char *buffer, std::size_t size) = 0;
        /**
         * @return the address of the executable that is running
         * @note this function is not signal safe, it allocates heap memory.
         */
        virtual std::string get_executable_path() = 0;
    };

    compatibility *get_compatibility_layer();
}

#endif // KHAROON_COMPATIBILITY_H
