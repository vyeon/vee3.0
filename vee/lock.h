#ifndef _VEE_LOCK_H_
#define _VEE_LOCK_H_
#include <mutex>
#include <atomic>

namespace vee {

namespace lock {

class empty_lock
{
public:
	inline static void lock()
	{
		
	};
	inline static bool try_lock()
	{
		return true;
	}
	inline static void unlock()
	{
		
	}
	inline static std::mutex::native_handle_type native_handle()
	{
		return nullptr;
	}
};

class spin_lock
{
	spin_lock(const spin_lock&) = delete; 
	void operator=(const spin_lock&) = delete;
	spin_lock(const spin_lock&&) = delete; 
	void operator=(const spin_lock&&) = delete;
public:
	spin_lock()
	{
		_lock.clear();
	}
	inline void lock(std::memory_order order = std::memory_order_acquire)
	{
		while (_lock.test_and_set(order));
	}
	inline bool try_lock(std::memory_order order = std::memory_order_acquire)
	{
		return _lock.test_and_set(order);
	}
	inline void unlock(std::memory_order order = std::memory_order_release)
	{
		_lock.clear(order);
	}
protected:
	std::atomic_flag _lock;
};

} // !namespace lock	

} // !namespace vee

#endif // !_VEE_LOCK_H_