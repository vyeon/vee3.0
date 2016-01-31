#ifndef _VEE_DELEGATE_H_
#define _VEE_DELEGATE_H_

#include <functional>
#include <vector>
#include <memory>
#include <mutex>
#include <map>
#include <vee/tupleupk.h>

namespace vee {

namespace exceptions {

class key_generation_failed
{
public:
    virtual const char* to_string()
    {
        return this->what();
    }
    const char* what()
    {
        return "function key gerneration failed";
    }
};

} // !namespace exceptions

namespace delegate_impl {

template <typename FuncSig, typename _Function>
bool compare_function(const _Function& lhs, const _Function& rhs)
{
    using target_type = typename ::std::conditional
        <
        ::std::is_function<FuncSig>::value,
        typename ::std::add_pointer<FuncSig>::type,
        FuncSig
        > ::type;
    if (const target_type* lhs_internal = lhs.template target<target_type>())
    {
        if (const target_type* rhs_internal = rhs.template target<target_type>())
            return *rhs_internal == *lhs_internal;
    }
    return false;
}

template <typename FTy>
class compareable_function: public ::std::function < FTy >
{
    using function_t = ::std::function<FTy>;
    bool(*_type_holder)(const function_t&, const function_t&);
public:
    compareable_function() = default;
    compareable_function(function_t& f):
        function_t(f),
        _type_holder(compare_function< FTy, function_t >)
    {

    }
    compareable_function(function_t&& f):
        function_t(::std::move(f)),
        _type_holder(compare_function< FTy, function_t >)
    {

    }
    template <typename CallableObj> compareable_function(CallableObj&& f):
        function_t(::std::forward<CallableObj>(f)),
        _type_holder(compare_function< ::std::remove_reference<CallableObj>::type, function_t >)
    {
        // empty
    }
    template <typename CallableObj> compareable_function& operator=(CallableObj&& f)
    {
        function_t::operator =(::std::forward<CallableObj>(f));
        _type_holder = compare_function < ::std::remove_reference<CallableObj>::type, function_t >;
        return *this;
    }
    friend bool operator==(const compareable_function& lhs, const compareable_function& rhs)
    {
        return rhs._type_holder(lhs, rhs);
    }
    /*friend bool operator==(const compareable_function &lhs, const function_t &rhs)
    {
        return rhs == lhs;
    }*/
};

} // !namespace delegate_impl

template <class FTy, 
          class LockTy = ::std::mutex, 
          class UsrKeyTy = int32_t >
class delegate
{

};

template <class RTy, 
          class ...Args, 
          class LockTy, 
          class UsrKeyTy >
class delegate < RTy(Args ...), LockTy, UsrKeyTy >
{
/* Define types and constant variables */
/* Define Public types */
public:
    using this_t = delegate<RTy(Args...), LockTy>;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::shared_ptr<this_t>;
    using args_tuple_t = ::std::tuple<Args...>;
    using lock_t = LockTy;
    using key_t = void*;
    using usrkey_t = UsrKeyTy;
    using binder_t = ::std::function<RTy(Args ...)>;

/* Define Private types */
private:
    using _cmpbinder_t = delegate_impl::compareable_function<RTy(Args ...)>;
    using _key_cmpbinder_pair = ::std::pair<usrkey_t, _cmpbinder_t>;
    using _seq_container_t = ::std::map<key_t, _cmpbinder_t>;
    using _usr_container_t = ::std::map<usrkey_t, _cmpbinder_t>;
    
/* Define Public static member functions */
public:
    inline static key_t gen_key(binder_t& dst) throw(...)
    {
        using target_type = typename ::std::conditional
            <
            ::std::is_function<RTy(Args...)>::value,
            typename ::std::add_pointer<RTy(Args...)>::type,
            RTy(Args...)
            > ::type;
        auto ptr = dst.template target<target_type>();
        if (!ptr)
            throw exceptions::key_generation_failed();
        printf("wrapper: %X, key: %X\n", ptr, *ptr);
        return mpl::pvoid_cast(*ptr);
    }

/* Define Public member functinos */
public:
    delegate() = default;
    ~delegate() = default;
    delegate(const ref_t other)
    {
        ::std::lock_guard<lock_t> locker(other._mtx);
        _cont = other._cont;
        _usrcont = other._usrcont;
    }
    delegate(rref_t other)
    {
        ::std::lock_guard<lock_t> locker(other._mtx);
        _cont = ::std::move(other._cont);
        _usrcont = ::std::move(other._usrcont);
    }
    ref_t operator=(const ref_t other)
    {
        // lock both mutexes without deadlock
        ::std::lock(_mtx, other._mtx);
        // make sure both already-locked mutexes are unlocked at the end of scope
        ::std::lock_guard<lock_t> locker1{ _mtx, ::std::adopt_lock };
        ::std::lock_guard<lock_t> locker2{ other._mtx, ::std::adopt_lock };

        _cont = other._cont;
        _usrcont = other._usrcont;

        return *this;
    }
    ref_t operator=(rref_t other)
    {
        // lock both mutexes without deadlock
        ::std::lock(_mtx, other._mtx);
        // make sure both already-locked mutexes are unlocked at the end of scope
        ::std::lock_guard<lock_t> locker1{ _mtx, ::std::adopt_lock };
        ::std::lock_guard<lock_t> locker2{ other._mtx, ::std::adopt_lock };

        _cont = ::std::move(other._cont);
        _usrcont = ::std::move(other._usrcont);

        return *this;
    }
    friend this_t operator+(const ref_t lhs, const ref_t rhs)
    {
        this_t ret{ lhs };
        ::std::lock_guard<lock_t> locker{ rhs._mtx };
        ret += rhs;
        return ret;
    }
    ref_t operator+=(const ref_t rhs)
    {
        // lock both mutexes without deadlock
        ::std::lock(_mtx, rhs._mtx);
        // make sure both already-locked mutexes are unlocked at the end of scope
        ::std::lock_guard<lock_t> locker1{ _mtx, ::std::adopt_lock };
        ::std::lock_guard<lock_t> locker2{ rhs._mtx, ::std::adopt_lock };

        for (auto& it : rhs._cont)
        {
            _cont.insert(it);
        }
        for (auto& it : rhs._usrcont)
        {
            _usrcont.insert(it);
        }

        return *this;
    }
    ref_t operator+=(rref_t rhs)
    {
        // lock both mutexes without deadlock
        ::std::lock(_mtx, rhs._mtx);
        // make sure both already-locked mutexes are unlocked at the end of scope
        ::std::lock_guard<lock_t> locker1{ _mtx, ::std::adopt_lock };
        ::std::lock_guard<lock_t> locker2{ rhs._mtx, ::std::adopt_lock };

        for (auto& it : rhs._cont)
        {
            _cont.insert(::std::move(it));
        }
        for (auto& it : rhs._usrcont)
        {
            _usrcont.insert(::std::move(it));
        }

        return *this;
    }
    template <class CallableObj>
    explicit delegate(CallableObj&& obj)
    {
        _cmpbinder_t cmpbinder{ ::std::forward<CallableObj>(obj) };
        _cont.insert(::std::make_pair(gen_key(cmpbinder), ::std::move(cmpbinder)));
    }
    template <class UsrKeyRef, class CallableObj>
    explicit delegate(UsrKeyRef&& key, CallableObj&& binder)
    {
        _cmpbinder_t cmpbinder{ ::std::forward<CallableObj>(binder) };
        _usrcont.insert(::std::make_pair(::std::forward<UsrKeyRef>(key), ::std::move(cmpbinder)));
    }
    template <class URef>
    ref_t operator+=(URef&& uref)
    {
        //mpl::meta_branch<::std::is_same<URef, >::type >;
    }
    template <class PairRef>
    ref_t _register(PairRef&& pair, mpl::meta_branch<true>/*is_tuple == true*/)
    {
        ::std::lock_guard<lock_t> locker{ _mtx };
        using PairTy = ::std::remove_reference<PairRef>::type;
        using UsrKeyRef = ::std::conditional< ::std::is_rvalue_reference<PairRef>::value,
            usrkey_t&&,
            usrkey_t >::type;
        using CallableObj = ::std::conditional< ::std::is_rvalue_reference<PairRef>::value,
            PairTy::second_type&&,
            PairTy::second_type >::type;
        _cmpbinder_t cmpbinder{ static_cast<CallableObj>(binder) };
        _usrcont.insert(::std::make_pair(static_cast<UsrKeyRef>(key), ::std::move(cmpbinder)));
        return *this;
    }
    template <class CallableObj>
    ref_t _register(CallableObj&& obj, mpl::meta_branch<false>/*is_tuple == false*/)
    {
        ::std::lock_guard<lock_t> locker{ _mtx };
        _cmpbinder_t cmpbinder{ ::std::forward<CallableObj>(obj) };
        _cont.insert(::std::make_pair(gen_key(cmpbinder), ::std::move(cmpbinder)));
        return *this;
    }

/* Define Protected member variables */
protected:
    lock_t _mtx;
    _seq_container_t _cont;
    _usr_container_t _usrcont;
};

template <class FTy,
          class LockTy = ::std::mutex,
          class UsrKeyTy = uint32_t >
delegate<FTy, LockTy, UsrKeyTy> make_delegate(::std::function<FTy> func)
{
    //TODO: Implementation
    return delegate<FTy, LockTy, UsrKeyTy >{ (func) };
}

template <class FTy,
          class LockTy = ::std::mutex,
          class UsrKeyTy = uint32_t,
          class Arg >
              delegate<FTy, LockTy, UsrKeyTy> make_delegate(Arg&& func)
{
    //TODO: Implementation
    return delegate<FTy, LockTy, UsrKeyTy >{ ::std::forward<Arg>(func) };
}

} // !namespace vee

#endif // !_VEE_DELEGATE_H_