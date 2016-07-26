#ifndef _VEE_EXCEPTION_H_
#define _VEE_EXCEPTION_H_

#include <vee/platform.h>
#include <vee/lib_base.h>
#include <array>
#include <exception>

namespace vee {

class exception: public std::exception
{
public:
    static const int desc_buffer_size = 256;
    std::array<char, desc_buffer_size> desc;
	using base_t = std::exception;
	exception();
	exception(const exception&) = default;
	explicit exception(char const* const);
	exception& operator=(const exception&) = default;
	virtual ~exception() = default;
	virtual char const* to_string() const noexcept;
};
	
} // !namespace vee

#endif // !_VEE_EXCEPTION_H_