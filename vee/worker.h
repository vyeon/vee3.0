#ifndef _VEE_WORKER_H_
#define _VEE_WORKER_H_

#include <vee/delegate.h>
#include <vee/lockfree/queue.h>
#include <thread>
#include <future>

namespace vee {

template <class FTy>
class packaged_task;

template <class RTy, class ...Args>
class packaged_task<RTy(Args...)> final
{
public:
    using this_t = packaged_task<RTy(Args...)>;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using delegate_t = delegate<RTy(Args...)>;
    using argstup_t = ::std::tuple< ::std::remove_reference_t<Args>... >;
    packaged_task() = default;
    template <class Delegate, class ...Arguments>
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
    void run()
    {
        if (!task.empty())
            task.operator()(args);
    }
    delegate_t task;
    argstup_t  args;
};

template <class FTy>
class worker;

#pragma warning(disable:4127)
template <class RTy, class ...Args>
class worker<RTy(Args ...)> final
{
public:
    using this_t = worker<RTy(Args...)>;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using delegate_t = delegate<RTy(Args...)>;
    using argstup_t = ::std::tuple<Args...>;
    using job_t = packaged_task<RTy(Args...)>;

    enum class state_t: int
    {
        standby = 0,
        running,
        shutdown
    };

    explicit worker(size_t __job_queue_size, bool autorun = true):
        job_queue_size { __job_queue_size },
        _job_queue { job_queue_size },
        _remained { 0 },
        _state { state_t::standby }
    {
        if (autorun)
        {
            start();
        }
    }
    ~worker()
    {
        shutdown(true);
    }
    /* Request a packaged_task to worker
       if request success, it returns the count of remained jobs
       if request faialed, it returns zero */
    template <class Job>
    size_t request(Job&& job)
    {
        bool result = _job_queue.enqueue(::std::forward<Job>(job));
        if (!result)
            return 0; // request failed, job queue is full
        size_t remained = _remained.fetch_add(1);
        if ((remained == 0) && (_state.load() != state_t::standby))
        {
            while (this->_promise == nullptr){ }; // waiting until promise ready
            ::std::promise<void>* promise = const_cast<::std::promise<void>*>(_promise);
            promise->set_value(); // signalling
        }
        return remained;
    }
    bool start()
    {
        state_t cmp{ state_t::standby };
        bool result = ::std::atomic_compare_exchange_strong(&_state, &cmp, state_t::running);
        if (result == false)
            return false; // worker is already in the running state
        _thr = ::std::thread{ &this_t::_worker_main, this };
        return true;
    }
    bool shutdown(bool sync)
    {
        state_t cmp{ state_t::running };
        bool result = ::std::atomic_compare_exchange_strong(&_state, &cmp, state_t::shutdown);
        if (result == false)
            return false; // worker isn't in the running state

        if (_remained.load() == 0)
        {
            // generate dummy job for wakeup the worker
            request(
                job_t{ }
            );
        }

        if (sync && _thr.joinable())
            _thr.join();
        else
            _thr.detach();
        return true;
    }

private:
    void _worker_main()
    {
        size_t remained = _remained.load();
        while (_state.load() == state_t::running)
        {
            if (remained == 0)
            {
                auto promise = new std::promise<void>();
                this->_promise = promise;
                auto future = promise->get_future();
                future.wait();
                _promise = nullptr;
                delete promise;
            }
            _epoch();
            remained = _remained.fetch_sub(1);
        }
        state_t cmp{ state_t::shutdown };
        bool result = ::std::atomic_compare_exchange_strong(&_state, &cmp, state_t::standby);
        if (result == false)
            throw ::std::runtime_error("unexpected worker state is detected while shutdown process");
    }

    job_t _current_job;
    bool _epoch()
    {
        if (!_job_queue.dequeue(_current_job))
            return false;
        _current_job.run();
        return true;
    }

public:
    const size_t job_queue_size;

private:
    ::std::atomic<size_t>  _remained;
    ::std::atomic<state_t> _state;
    ::std::promise<void> volatile* _promise;
    lockfree::queue<job_t> _job_queue;
    ::std::thread _thr;

private:
    // DISALLOW DEFAULT CONSTRUCTOR AND COPY AND MOVE OPERATIONS
    worker() = delete;
    worker(const ref_t) = delete;
    worker(rref_t) = delete;
    ref_t operator=(const ref_t) = delete;
    rref_t operator=(rref_t) = delete;
};

} // !namespace vee

#endif // !_VEE_WORKER_H_