#include <vee/io/stream.h>

namespace vee {

stream_base::~stream_base() noexcept
{
    on_destroy.do_call();
}

} // !namespace vee