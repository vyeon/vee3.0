#ifndef _VEE_LOCKFREE_QUEUE_H_
#define _VEE_LOCKFREE_QUEUE_H_

#include <array>
#include <atomic>
#include <vee/lockfree.h>

#pragma warning(disable:4127)

namespace vee {

namespace lockfree {

template <typename DataTy>
class queue final
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

template <typename DataTy>
class queue2 final
{
public:
	using data_t = DataTy;
	queue2(size_t capacity_):
		capacity { capacity_ },
		_cont_queue { capacity },
		_out_queue { capacity }
	{
		_cont = new data_t[capacity];
		for (size_t i = 0; i < capacity; )
		{
			if (_cont_queue.enqueue(i))
				++i;
			else
				throw ::std::runtime_error("queue2 initialization failed!");
		}
	}
	~queue2()
	{
		delete[] _cont;
	}
	template <typename DataRef>
	bool enqueue(DataRef&& data, size_t retries = 1000)
	{
		size_t counter = 0;

		while(counter < retries)
		{
			size_t block_id = 0;
			if (_cont_queue.dequeue(block_id))
			{
				_cont[block_id] = ::std::forward<DataRef>(data);
				while(!_out_queue.enqueue(block_id));
				return true;
			}
			else
			{
				++counter;
			}
		}
		return false;
	}

	bool dequeue(data_t& out)
	{
		size_t block_id = 0;
		if (!_out_queue.dequeue(block_id))
			return false;
		out = ::std::move(_cont[block_id]);
		while (!_cont_queue.enqueue(block_id));
		return true;
	}

	const size_t capacity;
private:
	data_t* _cont;
	queue<size_t> _cont_queue;
	queue<size_t> _out_queue;
};

} // !namespace lockfree

} // !namespace vee

#pragma warning(default:4127)

#endif // !_VEE_LOCKFREE_QUEUE_H_