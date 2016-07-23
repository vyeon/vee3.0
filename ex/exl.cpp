#include <vee/exl.h>

namespace vee
{

char const* precondition_violated_exception::to_string() const noexcept
{
	return base_t::to_string();
}

char const* key_generation_failed_exception::to_string() const noexcept
{
	return base_t::to_string();
}

char const* target_not_found_exception::to_string() const noexcept
{
	return base_t::to_string();
}

char const* key_already_exist_exception::to_string() const noexcept
{
	return base_t::to_string();
}

} // !namespace vee