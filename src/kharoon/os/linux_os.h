#ifndef KHAROON_LINUX_OS_H
#define KHAROON_LINUX_OS_H

#include "compatibility.h"
#include <vector>

namespace kharoon
{
    class linux_os : public compatibility
    {
    public:
        linux_os();
        virtual ~linux_os();
        /**
         * @brief dump_shared_libraries accesses the /proc/${PID}/maps file to read shared libraries loaded into the
         * process.
         * @post once all information is read or if there was an error opening maps file, fd will be set to unused
         * and the maps file will be closed.
         * @see compatilibity base class
         */
        virtual int dump_shared_libraries(char *buffer, std::size_t size) override;

        virtual int dump_hardware_information(char *buffer, std::size_t size) override;

        virtual std::string get_executable_path() override;

    protected:
        void initialize_device_info_vector();

    protected:
        using device_name_t = std::string;
        using device_info_files_t = std::pair<device_name_t, std::vector<std::string>>;
        using device_info_vector = std::vector<device_info_files_t>;
        device_info_vector device_infos;
        std::string pid_file;
        std::vector<std::string> cpu_temps;
        int fd;
    };
}



#endif // KHAROON_LINUX_OS_H
