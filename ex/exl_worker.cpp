#include <vee/worker.h>

namespace vee {

namespace exl {

char const* worker_is_busy::to_string() const __noexcept
{
    return this->what();
}

    
} // !namespace exl

} // !namespace vee