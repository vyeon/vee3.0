#ifndef _VEE_SIGNAL_H_
#define _VEE_SIGNAL_H_

#include <vee/libbase.h>
#include <future>

namespace vee {

using signal_status = ::std::future_status;

template <class T>
class p2psignal
{
	DISALLOW_COPY_AND_ASSIGN(p2psignal<T>);
	DISALLOW_MOVE_AND_ASSIGN(p2psignal<T>);
public:
	using this_t = p2psignal<T>;
	using ref_t = this_t&;
	using rref_t = this_t&&;
	p2psignal()
	{
		
	}
	~p2psignal()
	{
		
	}
	void wait(T& out)
	{
		::std::future<T> f = _p.get_future();
		f.wait();
		out = f.get();
	}
	template <class DurationTy>
	signal_status wait_for(T& out, DurationTy&& timeout)
	{
		::std::future<T> f = _p.get_future();
		auto status = f.wait_for(::std::forward<DurationTy>(timeout));
		if (status == ::std::future_status::ready)
		{
			out = f.get();
		}
		return status;
	}
	template <class DurationTy>
	signal_status wait_until(T& out, DurationTy&& timeout)
	{
		::std::future<T> f = _p.get_future();
		auto status = f.wait_until(::std::forward<DurationTy>(timeout));
		if (status == ::std::future_status::ready)
		{
			out = f.get();
		}
		return status;
	}
	template <class _T>
	void set_value(_T&& val)
	{
		_p.set_value(::std::forward<_T>(val));
	}
	template <class _T>
	void set_value_at_thread_exit(_T&& val)
	{
		_p.set_value_at_thread_exit(::std::forward<_T>(val));
	}
	void set_exception(::std::exception_ptr p)
	{
		_p.set_exception(p);
	}
	void set_exception_at_thread_exit(::std::exception_ptr p)
	{
		_p.set_exception_at_thread_exit(p);
	}
private:
	::std::promise<T> _p;
};

template <>
class p2psignal<void>
{
public:

};
	
} // !namespace vee

#endif // !_VEE_SIGNAL_H_