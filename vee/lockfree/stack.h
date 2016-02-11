#ifndef _VEE_LOCKFREE_STACK_H_
#define _VEE_LOCKFREE_STACK_H_

#include <vee/lockfree/queue.h>
#include <atomic>

#pragma warning(disable:4127)

namespace vee {

namespace lockfree {

template <typename DataTy>
class stack final
{
public:
	using this_t = stack<DataTy>;
	using ref_t = this_t&;
	using rref_t = this_t&&;
	using data_t = DataTy;
	explicit stack(size_t __capacity):
		capacity { __capacity },
		_cont_queue{ capacity },
		_top { 0 }
	{
		_cont = new data_t[capacity];
		_outarr = new data_t*[capacity];
		memset(_outarr, 0, sizeof(data_t*) * capacity);
		for (size_t i = 0; i < capacity; )
		{
			if (_cont_queue.enqueue(i))
				++i;
			else
				throw ::std::runtime_error("stack initialization failed!");
		}
	}
	~stack()
	{
		if (_cont)
			delete[] _cont;
		if (_outarr)
			delete[] _outarr;
	}
	template <typename DataRef>
	bool push(DataRef&& data)
	{
		::std::atomic_thread_fence(std::memory_order_release);
		size_t block_id = 0;
		if (_cont_queue.dequeue(block_id))
		{
			_cont[block_id] = ::std::forward<DataRef>(data);
			size_t outidx = _top.fetch_add(1);
			while (_outarr[outidx] != nullptr)
			{
				
			}
			_outarr[outidx] = &_cont[block_id];
		}
		else
		{
			return false; // stack is full
		}
		return true;
	}
	bool pop(data_t& out)
	{
		while (true)
		{
			std::atomic_thread_fence(std::memory_order_acquire);
			size_t top = _top.load();
			size_t dst = (top - 1);
			if (top == 0)
				return false; // stack is empty
			if (_outarr[dst] != nullptr)
			{
				if (::std::atomic_compare_exchange_strong(&_top, &top, dst))
				{
					using request_t = ::std::conditional_t<
						::std::is_move_assignable<data_t>::value,
						::std::add_rvalue_reference_t<data_t>,
						::std::add_lvalue_reference_t<data_t> >;
					out = static_cast<request_t>(_cont[dst]);
					_outarr[dst] = nullptr;
					return true;
				}
			}
		}
	}

	const size_t capacity;
private:
	data_t* _cont = nullptr;
	data_t** _outarr = nullptr;
	atqueue<size_t> _cont_queue;
	::std::atomic<size_t> _top;
};

} // !namespace lockfree
	
} // !namespace vee

#pragma warning(default:4127)

#endif // !_VEE_LOCKFREE_STATCK_H_