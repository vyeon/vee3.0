#ifndef _VEE_EXL_H_
#define _VEE_EXL_H_

#include <vee/exception.h>

namespace vee {
	
namespace exl {

class precondition_violated: public ::vee::exception
{
public:
	using base_t = ::vee::exception;
	precondition_violated():
		base_t{ "precondition violated" } { }
	virtual ~precondition_violated() = default;
	virtual char const* to_string() const __noexcept override;
};

class key_generation_failed: public ::vee::exception
{
public:
	using base_t = ::vee::exception;
	key_generation_failed():
		base_t{ "key gerneration failed" } { }
	virtual ~key_generation_failed() = default;
	virtual char const* to_string() const __noexcept override;
};

class target_not_found: public ::vee::exception
{
public:
	using base_t = ::vee::exception;
	target_not_found():
		base_t{ "target not found" } { }
	virtual ~target_not_found() = default;
	virtual char const* to_string() const __noexcept override;
};

class key_already_exist: public ::vee::exception
{
public:
	using base_t = ::vee::exception;
	key_already_exist():
		base_t{ "key already exist" } { }
	virtual ~key_already_exist() = default;
	virtual char const* to_string() const __noexcept override;
};

} // !namespace exl
	
} // !namespace vee

#endif // !_VEE_EXL_H_