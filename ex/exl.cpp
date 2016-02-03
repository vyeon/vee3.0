#include <vee/exl.h>

namespace vee
{
	
namespace exl
{

char const* key_generation_failed::to_string()
{
	return this->what();
}

char const* target_not_found::to_string()
{
	return this->what();
}

char const* key_already_exist::to_string()
{
	return this->what();
}

} // !namespace exl

} // !namespace vee