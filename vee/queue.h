#ifndef _VEE_QUEUE_H_
#define _VEE_QUEUE_H_

#include <vee/lock.h>

namespace vee {
	
template <class DataTy, class BlockLockTy = lock::spin_lock, class IndexLockTy = lock::spin_lock>
class queue
{
public:
	using this_t = queue<DataTy, IndexLockTy>;
	using ref_t = this_t&;
	using rref_t = this_t&&;
	using data_t = DataTy;
	using blocklock_t = BlockLockTy;
	using idxlock_t = IndexLockTy;
	
	explicit queue(const size_t capacity_, bool overwrite_flag = false):
		capacity { capacity_ },
		_overwrite_flag { overwrite_flag }
	{
		_blocks = new data_t[capacity];
		_blocklcks = new blocklock_t[capacity];
	}
	explicit queue(const ref_t other):
		queue{ other.capacity, other._overwrite_flag }
	{
		
	}
	virtual ~queue()
	{
		if (_blocks)
			delete[] _blocks;
		if (_blocklcks)
			delete[] _blocklcks;
	}
	inline bool is_empty() const
	{
		return (_size == 0);
	}
	inline bool is_full() const
	{
		return _size == capacity;
	}
	template <class DataRef>
	bool enqueue(DataRef&& val)
	{
		size_t rear;
		std::unique_lock<blocklock_t> block_locker;
		{
			std::lock_guard<idxlock_t> idx_locker{ _idxlck };
			rear = _rear;
			if(is_full())
			{
				if (!_overwrite_flag)
					return false;
				++_rear %= capacity;
				++_front %= capacity;
			}
			else
			{
				++_rear %= capacity;
				++_size;
			}
			std::unique_lock<blocklock_t> temp{_blocklcks[rear], std::adopt_lock};
			std::swap(block_locker, temp);
		}
		_blocks[rear] = std::forward<DataRef>(val);
		return true;
	}
	bool dequeue(data_t& out)
	{
		size_t front;
		std::unique_lock<blocklock_t> block_locker;
		{
			std::lock_guard<idxlock_t> idx_locker{ _idxlck };
			if (!_size)
				return false;
			--_size;
			front = _front;
			++_front %= capacity;

			std::unique_lock<blocklock_t> temp{ _blocklcks[front], std::adopt_lock };
			std::swap(block_locker, temp);
		}
		using request_t = std::conditional_t<
			std::is_trivially_move_assignable<data_t>::value,
			std::add_rvalue_reference_t<data_t>,
			std::add_lvalue_reference_t<data_t> >;
		out = static_cast<request_t>(_blocks[front]);
		return true;
	}
	const size_t capacity;
private:
	data_t* _blocks = nullptr;
	blocklock_t* _blocklcks = nullptr;
	idxlock_t  _idxlck;
	bool    _overwrite_flag = false;
	size_t  _front = 0;
	size_t  _rear = 0;
	size_t  _size = 0;
};

} // !namespace vee

#endif // !_VEE_QUEUE_H_