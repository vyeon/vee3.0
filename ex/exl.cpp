#include <vee/exl.h>

namespace vee
{
	
namespace exl
{

char const* precondition_violated::to_string() const __noexcept
{
	return this->what();
}

char const* key_generation_failed::to_string() const __noexcept
{
	return this->what();
}

char const* target_not_found::to_string() const __noexcept
{
	return this->what();
}

char const* key_already_exist::to_string() const __noexcept
{
	return this->what();
}

} // !namespace exl

} // !namespace vee