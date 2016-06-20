#include <vee/io/stream.h>

namespace vee {
    
namespace io {

invalid_stream_exception::invalid_stream_exception(char const* const msg):
    base_t{ msg }
{
}

char const* invalid_stream_exception::to_string() const __noexcept
{
    return this->what();
}

stream_write_failed_exception::stream_write_failed_exception(char const* const msg):
    base_t{ msg }
{
}

char const* stream_write_failed_exception::to_string() const __noexcept
{
    return this->what();
}

stream_reset_exception::stream_reset_exception(char const* const msg):
    base_t{ msg }
{
}

char const* stream_reset_exception::to_string() const __noexcept
{
    return this->what();
}

unknown_io_exception::unknown_io_exception(char const* const msg):
    base_t { msg }
{
}

char const* unknown_io_exception::to_string() const __noexcept
{
    return this->what();
}

} // !namespace io

} // !namespace vee