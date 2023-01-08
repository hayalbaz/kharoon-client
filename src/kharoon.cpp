#include <kharoon/client/kharoon.h>
#include "kharoon/handler.h"
#include "kharoon/context.h"
#include <cstring>

void kharoon_client_init()
{
    using namespace kharoon;
    context::get();
}

void kharoon_add_object_to_dump(const char* obj)
{
    using namespace kharoon;
    context::get()->add_new_object(obj);
}

void kharoon_client_dump_unwind_stdout()
{
    kharoon_dump_unwind(1);
}


