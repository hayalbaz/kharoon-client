#include "compatibility.h"
#include "linux_os.h"

namespace kharoon
{
    compatibility *get_compatibility_layer()
    {
#if defined(_MSC_VER)
    #pragma error Windows compatibility layer have not been implemented yet.
#elif defined(__GNUC__)
    return new linux_os();
#else
    #pragma error Unknown operating system, do not have a compatibility layer.
#endif
    }
}
