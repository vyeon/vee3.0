#ifndef _VEE_EXL_H_
#define _VEE_EXL_H_

#include <vee/exception.h>

namespace vee {
	
class precondition_violated_exception: public vee::exception
{
public:
	using base_t = vee::exception;
	precondition_violated_exception():
		base_t{ "precondition violated exception" } { }
	virtual ~precondition_violated_exception() = default;
	virtual char const* to_string() const noexcept override;
};

class key_generation_failed_exception: public vee::exception
{
public:
	using base_t = vee::exception;
	key_generation_failed_exception():
		base_t{ "key gerneration failed exception" } { }
	virtual ~key_generation_failed_exception() = default;
	virtual char const* to_string() const noexcept override;
};

class target_not_found_exception: public vee::exception
{
public:
	using base_t = vee::exception;
	target_not_found_exception():
		base_t{ "target not found exception" } { }
	virtual ~target_not_found_exception() = default;
	virtual char const* to_string() const noexcept override;
};

class key_already_exist_exception: public vee::exception
{
public:
	using base_t = vee::exception;
	key_already_exist_exception():
		base_t{ "key already exist exception" } { }
	virtual ~key_already_exist_exception() = default;
	virtual char const* to_string() const noexcept override;
};

} // !namespace vee

#endif // !_VEE_EXL_H_