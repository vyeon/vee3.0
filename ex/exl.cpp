#include <vee/exl.h>

namespace vee
{

char const* precondition_violated_exception::to_string() const __noexcept
{
	return this->what();
}

char const* key_generation_failed_exception::to_string() const __noexcept
{
	return this->what();
}

char const* target_not_found_exception::to_string() const __noexcept
{
	return this->what();
}

char const* key_already_exist_exception::to_string() const __noexcept
{
	return this->what();
}

} // !namespace vee