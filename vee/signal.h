#ifndef _VEE_SIGNAL_H_
#define _VEE_SIGNAL_H_

#include <vee/libbase.h>
#include <future>

namespace vee {

template <class T>
class sigch
{
	DISALLOW_COPY_AND_ASSIGN(sigch<T>);
	DISALLOW_MOVE_AND_ASSIGN(sigch<T>);
public:
	using this_t = sigch<T>;
	using ref_t = this_t&;
	using rref_t = this_t&&;
	sigch()
	{
		
	}
	~sigch()
	{
		
	}

private:
	::std::future<T> _future;

};

template <>
class sigch<void>
{
public:

};
	
} // !namespace vee

#endif // !_VEE_SIGNAL_H_