#ifndef _VEE_WORKER_H_
#define _VEE_WORKER_H_

#include <vee/delegate.h>
#include <vee/lockfree/stack.h>
#include <vee/exception.h>
#include <thread>
#include <future>
#include <list>

namespace vee {

namespace exl {
    
class worker_is_busy: public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    worker_is_busy():
        base_t{ "worker is busy" }
    {
    }
    virtual ~worker_is_busy() = default;
    virtual char const* to_string() const noexcept override;
};

} // !namespace exl

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
    using argstup_t = std::tuple< std::remove_reference_t<Args>... >;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    packaged_task() = default;
    packaged_task(ref_t other):
        task{ other.task },
        args{ other.args },
        is_valid{ other.is_valid }
    {
        
    }
    packaged_task(rref_t other):
        task{ std::move(other.task) },
        args{ std::move(other.args) },
        is_valid{ std::move(other.is_valid) }
    {
        
    }
    template <class Delegate, class ...Arguments>
    explicit packaged_task(Delegate&& e, Arguments&& ...args):
        task{ std::forward<Delegate>(e) },
        args{ std::make_tuple(std::forward<Arguments>(args)...) },
        is_valid { true }
    {

    }
    template <class Delegate>
    explicit packaged_task(Delegate&& e, argstup_t&& tup):
        task{ std::forward<Delegate>(e) },
        args{ std::move(tup) },
        is_valid { true }
    {

    }
    template <class Delegate>
    explicit packaged_task(Delegate&& e, argstup_t& tup):
        task{ std::forward<Delegate>(e) },
        args{ tup },
        is_valid { true }
    {

    }
    ~packaged_task()
    {
        //puts(__FUNCTION__);
    }
    ref_t operator=(const ref_t rhs)
    {
        task = rhs.task;
        args = rhs.args;
        is_valid = rhs.is_valid;
        return *this;
    }
    ref_t operator=(rref_t rhs)
    {
        task = std::move(rhs.task);
        args = std::move(rhs.args);
        is_valid = rhs.is_valid;
        return *this;
    }
    void run()
    {
        if (!task.empty())
            task.operator()(args);
    }
    delegate_t task;
    argstup_t  args;
    volatile bool is_valid;
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
    using argstup_t = std::tuple<Args...>;
    using task_t = packaged_task<RTy(Args...)>;
    using job_t = typename task_t::shared_ptr;

    struct events_wrapper
    {
        using sleep_event_t = delegate<void(), lock::spin_lock>;
        sleep_event_t sleep;
        using job_processed_event_t = delegate<void(), lock::spin_lock>;
        job_processed_event_t job_processed;
        using job_requested_event_t = delegate<void(), lock::spin_lock>;
        job_requested_event_t job_requested;
    };

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
        size_t result = nothrow_request(std::forward<Job>(job));
        if (!result)
            throw worker_is_busy{};
        return result;
    }
    template <class Job>
    size_t nothrow_request(Job&& job)
    {
        bool result = _job_queue.enqueue(std::forward<Job>(job));
        if (!result)
            return 0; // request failed, job queue is full
        size_t remained_old = _remained.fetch_add(1);
        if ((remained_old == 0) && (_state.load() != state_t::standby))
        {
            while (this->_promise == nullptr)
            {
            }; // waiting until promise ready
            _promise->set_value(); // signalling
        }
        events.job_requested.operator()();
        return remained_old + 1;
    }
    bool start()
    {
        state_t cmp{ state_t::standby };
        bool result = std::atomic_compare_exchange_strong(&_state, &cmp, state_t::running);
        if (result == false)
            return false; // worker is already in the running state
        _thr = std::thread{ &this_t::_worker_main, this };
        return true;
    }
    bool shutdown(bool sync)
    {
        state_t cmp{ state_t::running };
        bool result = std::atomic_compare_exchange_strong(&_state, &cmp, state_t::shutdown);
        if (result == false)
            return false; // worker isn't in the running state

        if (_remained.load() == 0)
        {
            // generate a dummy job for wakeup the worker
            request(
                std::make_shared<task_t>()
            );
        }

        if (sync && _thr.joinable())
            _thr.join();
        else
            _thr.detach();
        return true;
    }
    size_t guess_remined_jobs() const
    {
        return _remained.load();
    }
    state_t guess_state() const
    {
        return _state.load();
    }

private:
    void _worker_main()
    {
        size_t remained = _remained.load();
        while (_state.load() == state_t::running)
        {
            if (remained == 0)
            {
                auto promise = new std::make_shared<std::promise<void>>();
                //auto promise = new std::promise<void>();
                this->_promise = promise;
                auto future = promise->get_future();
                events.sleep.operator()();
                puts("worker wait");
                future.wait();
                _promise.reset();
                //delete promise;
            }
            _epoch();
            remained = _remained.fetch_sub(1) - 1;
        }
        state_t cmp{ state_t::shutdown };
        bool result = std::atomic_compare_exchange_strong(&_state, &cmp, state_t::standby);
        if (result == false)
            throw std::runtime_error("unexpected worker state is detected while shutdown process");
        puts("worker main exit");
    }

    job_t _current_job;
    bool _epoch()
    {
        if (!_job_queue.dequeue(_current_job))
            return false;
        _current_job->run();
        _current_job.reset();
        events.job_processed.operator()();
        return true;
    }

public:
    const size_t job_queue_size;
    events_wrapper events;

private:
    std::atomic<size_t>  _remained;
    std::atomic<state_t> _state;
    std::shared_ptr<std::promise<void>> _promise;
    lockfree::queue<job_t> _job_queue;
    std::thread _thr;

private:
    // DISALLOW DEFAULT CONSTRUCTOR AND COPY AND MOVE OPERATIONS
    worker() = delete;
    worker(const ref_t) = delete;
    worker(rref_t) = delete;
    ref_t operator=(const ref_t) = delete;
    rref_t operator=(rref_t) = delete;
};

template <class FTy>
class nonscalable_worker_group;

template <class RTy, class ...Args>
class nonscalable_worker_group<RTy(Args ...)>
{
    using worker_handle = std::shared_ptr<worker<RTy(Args ...)>>;
public:
    using worker_t = worker<RTy(Args ...)>;
    using this_t = nonscalable_worker_group<RTy(Args...)>;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using delegate_t = delegate<RTy(Args...)>;
    using argstup_t = std::tuple<Args...>;
    using job_t = packaged_task<RTy(Args...)>;
    using index_t = size_t;

    explicit nonscalable_worker_group(size_t __number_of_workers, size_t __job_queue_size):
        total_job_queue_capacity{ __number_of_workers * __job_queue_size },
        number_of_workers { __number_of_workers },
        /*_stack { __number_of_workers },*/
        _job_counter { 0 }
    {
        _workers.reserve(__number_of_workers);
        /*_stackables.reserve(__number_of_workers);*/
        for (size_t i = 0; i < __number_of_workers; ++i)
        {
            _workers.push_back( std::make_shared<worker_t>(__job_queue_size, false)/*autorun*/ );
            /*_stackables.push_back( std::make_shared<std::atomic_flag>() );*/

            _workers[i]->events.sleep += std::make_pair(i, std::bind(&this_t::on_worker_sleep, this, i));
            _workers[i]->events.job_requested += std::make_pair(i, std::bind(&this_t::on_job_requested, this, i));
            _workers[i]->events.job_processed += std::make_pair(i, std::bind(&this_t::on_job_processed, this, i));
            _workers[i]->start();
        }
    }

    ~nonscalable_worker_group()
    {
        for (auto& it : _workers)
        {
            it->shutdown(false);
        }
    }

    void on_worker_sleep(index_t /*id*/)
    {
        //if (!_stackables[id]->test_and_set())
        //{
        //    if (!_stack.push(id))
        //        throw std::runtime_error("stack::push failed in the worker group!");
        //    else
        //        puts("success to store worker to group stack"); // logs for debug
        //}
    }

    void on_job_processed(index_t id)
    {
        printf("Worker %u comsumed the job\n", id);
        _job_counter.fetch_sub(1);
    }

    void on_job_requested(index_t id)
    {
        printf("Worker %u accepted the job\n", id);
        _job_counter.fetch_add(1);
    }

    template <class JobRef>
    bool request(JobRef&& job)
    {
        bool result = nothrow_request(std::forward<JobRef>(job));
        if (!result)
            throw worker_is_busy{};
        return true;
    }

    template <class JobRef>
    bool nothrow_request(JobRef&& job)
    {
        /*index_t id;
        if (_stack.pop(id))
        {
            _stackables[id]->clear();
            return _workers[id]->request(std::forward<JobRef>(job));
        }*/
        // Add the schedule algorithms -> current: temporary linear search algorithm
        long double average = static_cast<long double>(_job_counter.load() / number_of_workers);
        for (index_t id = 0; id < number_of_workers; ++id)
        {
            if (_workers[id]->guess_remined_jobs() <= average)
            {
                size_t result = _workers[id]->nothrow_request(std::forward<JobRef>(job));
                if (result)
                    return true;
            }
        }
        return false; // all of workers are busy
    }


    const size_t total_job_queue_capacity;
    const size_t number_of_workers;
private:
    std::vector<worker_handle> _workers;
    /*lockfree::stack<index_t> _stack;
    std::vector< std::shared_ptr< std::atomic_flag > > _stackables;*/
    std::atomic<size_t> _job_counter;

    // DISALLOW DEFAULT CONSTRUCTOR AND COPY & MOVE OPERATIONS
    nonscalable_worker_group() = delete;
    nonscalable_worker_group(const ref_t) = delete;
    nonscalable_worker_group(rref_t) = delete;
    ref_t operator=(const ref_t) = delete;
    ref_t operator=(rref_t) = delete;
};

} // !namespace vee

#endif // !_VEE_WORKER_H_