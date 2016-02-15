#ifndef _VEE_WORKER_H_
#define _VEE_WORKER_H_

#include <thread>
#include <vee/delegate.h>
#include <vee/lockfree/queue.h>

namespace vee {

template <class FTy>
class packaged_task;

template <class RTy, class ...Args>
class packaged_task
{
    packaged_task() = delete;
public:
    using this_t = packaged_task<RTy(Args...)>;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using delegate_t = delegate<RTy(Args...)>;
    using argstup_t = ::std::tuple<Args...>;
    template <class Delegate, class Arguments>
    explicit packaged_task(Delegate&& e, Arguments&& ...args):
        task{ ::std::forward<Delegate>(e) },
        args{ ::std::make_tuple(::std::forward<Arguments>(args)...) }
    {

    }
    template <class Delegate>
    explicit packaged_task(Delegate&& e, argstup_t&& tup):
        task{ ::std::forward<Delegate>(e) },
        args{ ::std::move(tup) }
    {

    }
    template <class Delegate>
    explicit packaged_task(Delegate&& e, argstup_t& tup):
        task{ ::std::forward<Delegate>(e) },
        args{ tup }
    {

    }
    ~packaged_task()
    {

    }
    ref_t operator=(const ref_t rhs)
    {
        task = rhs.task;
        args = rhs.args;
        return *this;
    }
    ref_t operator=(rref_t rhs)
    {
        task = ::std::move(rhs.task);
        args = ::std::move(rhs.args);
        return *this;
    }
    delegate_t task;
    argstup_t  args;
};

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
    using job_t = packaged_task<RTy(Args...)>;
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
    /* Request a packaged_task to worker
       if request success, it returns the count of remained jobs
       if request faialed, it returns zero */
    template <class Job>
    size_t request(Job&& job)
    {
        bool result = _job_queue.enqueue(::std::forward<Job>(job));
        if (!result)
            return 0;
        size_t remained = _remained.fetch_add(1);
        return remained;
    }
    const size_t job_queue_size;
private:
    ::std::atomic<size_t> _remained;
    lockfree::queue<job_t> _job_queue;
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