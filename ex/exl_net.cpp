#include <vee/net.h>

namespace vee {

namespace exl {

namespace net {

inline connection_failed::connection_failed(char const* const msg):
    base_t{ msg }
{
}

char const* connection_failed::to_string() const __noexcept
{
    return this->what();
}

} // !namespace net
    
} // !namespace exl

} // !namespace vee