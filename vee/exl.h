#ifndef _VEE_EXL_H_
#define _VEE_EXL_H_

#include <vee/exception.h>

namespace vee {
	
namespace exl {

class key_generation_failed: public ::vee::exception
{
public:
	using base_t = ::vee::exception;
	key_generation_failed():
		base_t{ "key gerneration failed" }
	{
	}
	virtual ~key_generation_failed() = default;
	virtual char const* to_string() override;
};

class target_not_found: public ::vee::exception
{
public:
	using base_t = ::vee::exception;
	target_not_found():
		base_t{ "target not found" }
	{
	}
	virtual ~target_not_found() = default;
	virtual char const* to_string() override;
};

class key_already_exist: public ::vee::exception
{
public:
	using base_t = ::vee::exception;
	key_already_exist():
		base_t{ "key already exist" }
	{
	}
	virtual ~key_already_exist() = default;
	virtual char const* to_string() override;
};

} // !namespace exl
	
} // !namespace vee

#endif // !_VEE_EXL_H_