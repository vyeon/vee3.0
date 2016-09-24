#include <vee/io.h>

namespace vee {

invalid_stream_exception::invalid_stream_exception(char const* const msg):
    base_t{ msg }
{
}

char const* invalid_stream_exception::to_string() const noexcept
{
    return base_t::to_string();
}

stream_write_failed_exception::stream_write_failed_exception(char const* const msg):
    base_t{ msg }
{
}

char const* stream_write_failed_exception::to_string() const noexcept
{
    return base_t::to_string();
}

stream_reset_exception::stream_reset_exception(char const* const msg):
    base_t{ msg }
{
}

char const* stream_reset_exception::to_string() const noexcept
{
    return base_t::to_string();
}

unknown_io_exception::unknown_io_exception(char const* const msg):
    base_t { msg }
{
}

char const* unknown_io_exception::to_string() const noexcept
{
    return base_t::to_string();
}

} // !namespace vee