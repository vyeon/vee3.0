#ifndef _VEE_LOCKFREE_QUEUE_H_
#define _VEE_LOCKFREE_QUEUE_H_

#include <cstdint>
#include <array>
#include <atomic>
#include <vee/lockfree.h>

#pragma warning(disable:4127)

namespace vee {

namespace lockfree {

template <typename DataTy>
class ptrqueue
{
public:

};

template <typename DataTy>
class queue
{
public:
	using data_t = DataTy;
	explicit queue(size_t capacity_):
		capacity { capacity_ },
		_front { 0 },
		_rear { 0 }
	{
		_cont = new data_t[capacity];
		_ptrs = new data_t*[capacity];
		memset(_ptrs, 0, sizeof(data_t*) * capacity);
	}
	~queue()
	{
		delete[] _cont;
		delete[] _ptrs;
	}
	template <typename DataRef>
	bool enqueue(DataRef&& value, size_t retries = 1000)
	{
		size_t counter = 0;

		while (counter < retries)
		{
			::std::atomic_thread_fence(std::memory_order_release);
			size_t front = _front.load();
			if (_ptrs[front] == nullptr)
			{
				size_t next = (front + 1) % capacity;
				if (::std::atomic_compare_exchange_strong(&_front, &front, next))
				{
					_cont[front] = ::std::forward<DataRef>(value);
					_ptrs[front] = &_cont[front];
					return true;
				}
			}
			else
			{
				// Queue is full
				++counter;
			}
		}
		return false;
	}

	bool dequeue(data_t& out)
	{
		while (true)
		{
			std::atomic_thread_fence(std::memory_order_acquire);
			size_t rear = _rear.load();
			if(_ptrs[rear] != nullptr)
			{
				size_t next = (rear + 1) % capacity;
				if(::std::atomic_compare_exchange_strong(&_rear, &rear, next))
				{
					out = ::std::move(_cont[rear]);
					_ptrs[rear] = nullptr;
					return true;
				}
			}
			else
			{
				// Queue is empty
				break;
			}
		}
		return false;
	}
	
	const size_t capacity;
private:
	::std::atomic<size_t> _front; // enqueue index
	::std::atomic<size_t> _rear;  // dequeue index
	data_t* _cont;
	data_t** _ptrs;
};

} // !namespace lockfree

} // !namespace vee

#pragma warning(default:4127)

#endif // !_VEE_LOCKFREE_QUEUE_H_