#include <vee/worker.h>

namespace vee {

char const* worker_is_busy::to_string() const noexcept
{
    return base_t::to_string();
}

} // !namespace vee