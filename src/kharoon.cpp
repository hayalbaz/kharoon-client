#include <kharoon/client/kharoon.h>
#include "kharoon/context.h"
#include <cstring>

using namespace kharoon;

int kharoon_client_init_start()
{
    init_in_progress = 1;
    context::get()->disable_crash_handler();
    return 0;
}

int kharoon_client_init_end()
{
    context::get()->setup_crash_handler();
    init_in_progress = 0;
    return 0;
}

void kharoon_add_object_to_dump(const char* obj)
{
    if (!init_in_progress) {
        return;
    }
    context::get()->add_new_object(obj);
}
