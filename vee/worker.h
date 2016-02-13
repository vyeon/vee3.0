#ifndef _VEE_WORKER_H_
#define _VEE_WORKER_H_

#include <thread>
#include <vee/delegate.h>
#include <vee/lockfree/queue.h>

namespace vee {

namespace worker_impl {

template <class FTy>
class packaged_task;

template <class RTy, class ...Args>
class packaged_task
{
public:
	using this_t = packaged_task<RTy(Args...)>;
	using ref_t = this_t&;
	using rref_t = this_t&&;
	using delegate_t = delegate<RTy(Args...)>;
	using argstup_t = ::std::tuple<Args...>;
	//template <class>
	//explicit packaged_task()
	//{
	//	
	//}
};
	
} // !namespace worker_impl

template <class FTy>
class worker;

#pragma warning(disable:4127)
template <class RTy, class ...Args>
class worker<RTy(Args ...)>
{
public:
	using this_t = worker<RTy(Args...)>;
	using ref_t = this_t&;
	using rref_t = this_t&&;
	using delegate_t = delegate<RTy(Args...)>;
	using argstup_t = ::std::tuple<Args...>;
	//using job_t = ::std::pair<delegate_t, argstup_t>;
	explicit worker(size_t job_queue_size_, bool autorun = true):
		job_queue_size { job_queue_size_ }
	{
		if (autorun)
		{
			
		}
	}
	~worker()
	{
			
	}
	template <typename Delegate>
	bool request(Delegate&& task, Args&& ...args)
	{
		
	}
	const size_t job_queue_size;
private:
	/*::std::atomic<size_t> _remained;
	lockfree::queue<job_t> _job_queue;*/
	::std::thread _thr;

private:
	worker() = delete;
	worker(const ref_t) = delete;
	worker(rref_t) = delete;
	ref_t operator=(const ref_t) = delete;
	rref_t operator=(rref_t) = delete;
};

} // !namespace vee

#endif // !_VEE_WORKER_H_