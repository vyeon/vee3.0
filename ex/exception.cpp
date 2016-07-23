#include <vee/exception.h>
#include <cstring>

namespace vee {
	
exception::exception()
{
    desc.fill(0);
    desc[0] = 'n';
    desc[1] = 'u';
    desc[2] = 'l';
    desc[3] = 'l';
}

exception::exception(char const* const msg)
{
    desc.fill(0);
    strcpy_s(desc.data(), desc_buffer_size, msg);
}

char const* exception::to_string() const noexcept
{
    return desc.data();
}

} // !namespace vee