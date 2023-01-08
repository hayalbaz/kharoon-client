#ifndef KHAROON_HANDLER_H
#define KHAROON_HANDLER_H

#define UNW_LOCAL_ONLY
#include <libunwind.h>

void kharoon_handler(int signum);

void kharoon_dump_unwind(int fd);

void kharoon_dump_shared_libraries(int fd);

void kharoon_dump_system_information(int fd);


#endif // KHAROON_HANDLER_H
