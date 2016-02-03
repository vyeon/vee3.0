#ifndef _VEE_EXCEPTION_H_
#define _VEE_EXCEPTION_H_

#include <vee/libbase.h>
#include <exception>

namespace vee {

class exception: public ::std::exception
{
public:
	using base_t = ::std::exception;
	exception();
	explicit exception(char const* const);
	explicit exception(const exception&) = default;
	exception& operator=(const exception&) = default;
	virtual ~exception() = default;
	virtual char const* to_string() __PURE;
};
	
} // !namespace vee

#endif // !_VEE_EXCEPTION_H_