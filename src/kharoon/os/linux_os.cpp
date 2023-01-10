#include "linux_os.h"
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include <fstream>

namespace kharoon
{
    linux_os::linux_os()
    {
        initialize_device_info_vector();
        pid_file = "/proc/" + std::to_string(getpid()) + "/maps";
        fd = -1;
    }

    linux_os::~linux_os()
    {
        if (fd != -1) {
            close(fd);
        }
    }

    int linux_os::dump_shared_libraries(char *buffer, std::size_t size)
    {
        if (fd == -1) {
            fd = open(pid_file.c_str(), O_RDONLY);
            if (fd < 0) {
                fd = -1;
                return -1;
            }
        }

        auto res = read(fd, buffer, size);
        if (res == 0) {
            close(fd);
            fd = -1;
        }

        return res;
    }

    int linux_os::dump_hardware_information(char *buffer, std::size_t size)
    {
        return 0;
    }

    std::string linux_os::get_executable_path()
    {
        return "/proc/" + std::to_string(getpid()) + "/exe";
    }

    void linux_os::initialize_device_info_vector()
    {
        namespace fs = std::filesystem;
        const fs::path hwmon{"/sys/class/hwmon"};
        for (const auto &mon : fs::directory_iterator{hwmon})
        {
            if (!mon.is_directory()) {
                continue;
            }

            auto name_file = mon.path() / "name";
            std::ifstream ifs(name_file);
            if (!ifs.is_open()) {
                continue;
            }
            std::string name;
            ifs >> name;

            device_info_files_t files{name, {}};
            for (const auto &f : fs::directory_iterator{mon}) {
                if (!f.is_regular_file()) {
                    continue;
                }

                auto is_temperature_file = f.path().generic_string().find("temp") != std::string::npos;
                if (!is_temperature_file) {
                    continue;
                }
                files.second.push_back(f.path());
            }
            device_infos.push_back(files);
        }
    }
}

