#ifndef _VEE_TUPLEUPK_H_
#define _VEE_TUPLEUPK_H_

#include <vee/mpl.h>
#include <tuple>
#include <functional>

namespace vee {

namespace tupleupk_impl {

template <class FTy>
struct function_parser
{
};

template <class RTy, typename ...Args>
struct function_parser < RTy(Args ...) >
{
    typedef RTy return_type;
    typedef ::std::tuple<Args ...> argstuple_type;
    template <int INDEX>
    struct argtype
    {
        template <::std::size_t N>
        using type = typename ::std::tuple_element<N, ::std::tuple<Args...>>::type;
    };
};

template <class FuncSig> // parse ::std::function type
struct function_parser< ::std::function<FuncSig> >: function_parser<FuncSig>
{
};

template <class RTy, class CallableObj, class Tuple, int ...S>
inline RTy _do_call(CallableObj&& func, Tuple&& tuple, mpl::sequence<S...>)
{
    return func(::std::forward<decltype(::std::get<S>(tuple))>(::std::get<S>(tuple)) ...);
}

} // !namespace tupleupk_impl

template <
    class CallableObj,
    class Tuple,
    class RTy = tupleupk_impl::function_parser< ::std::remove_reference<CallableObj>::type >::return_type >
        RTy tupleupk(CallableObj&& func, Tuple&& tuple)
    {
        return tupleupk_impl::_do_call<RTy>(std::forward<CallableObj>(func),
                                              ::std::forward<Tuple>(tuple),
                                              typename mpl::sequence_generator< ::std::tuple_size< typename ::std::remove_reference<Tuple>::type >::value/*sizeof...(Args)*/>::type());
    }

#pragma warning(default:4100)

} // !namespace vee

#endif // !_VEE_TUPLEUPK_H_
