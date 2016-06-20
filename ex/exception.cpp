#include <vee/exception.h>

namespace vee {
	
exception::exception():
	base_t { "Undefined exception" }
{
	
}

exception::exception(char const* const msg):
	base_t { msg }
{

}

char const* exception::to_string() const noexcept
{
    return this->what();
}

} // !namespace vee