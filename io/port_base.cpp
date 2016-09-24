#include <vee/io/port_base.h>

namespace vee {

namespace io {

FILE* base_in = stdin;
FILE* base_out = stdout;
FILE* base_err = stderr;

port_base::~port_base() noexcept
{
}

} // !namespace io

} // !namespace vee