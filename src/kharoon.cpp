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
    context::get()->add_new_object(obj);
}

int kharoon_add_metadata_to_dump(const char *key, const void *metadata, size_t sz)
{
    return context::get()->add_metadata(key, metadata, sz) ? 0 : 1;
}

void kharoon_set_restart_after_crash()
{
    context::get()->set_restart_on_crash(true);
}

void kharoon_reset_restart_after_crash()
{
    context::get()->set_restart_on_crash(false);
}

void kharoon_set_dump_system_environment()
{
    context::get()->set_dump_system_environment(true);
}

void kharoon_reset_dump_system_environment()
{
    context::get()->set_dump_system_environment(false);
}

void kharoon_set_dump_hardware_information()
{
    context::get()->set_dump_hardware_information(true);
}

void kharoon_reset_dump_hardware_information()
{
    context::get()->set_dump_hardware_information(false);
}
