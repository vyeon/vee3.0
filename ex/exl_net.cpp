#include <vee/net.h>

namespace vee {

namespace net {

inline connection_failed_exception::connection_failed_exception(char const* const msg):
    base_t{ msg }
{
}

char const* connection_failed_exception::to_string() const noexcept
{
    return base_t::to_string();
}

char const* protocol_mismatch_exception::to_string() const noexcept
{
    return base_t::to_string();
}

char const* connection_already_disconnected::to_string() const noexcept
{
    return base_t::to_string();
}

char const* accept_failed_exception::to_string() const noexcept
{
    return base_t::to_string();
}

} // !namespace net

} // !namespace vee