#ifndef _VEE_MPL_H_
#define _VEE_MPL_H_

#include <utility>

namespace vee {

namespace mpl {

template<int ...>
struct sequence
{
};

template<int N, int ...S>
struct sequence_generator: sequence_generator < N - 1, N - 1, S... >
{
};

template<int ...S>
struct sequence_generator < 0, S... >
{
    typedef sequence<S...> type;
};

template <class T>
void* pvoid_cast(T pointer)
{
    auto& ptr = pointer;
    void* addr = *(void**)(&ptr);
    return addr;
}

template <int I>
struct int_to_type
{
    enum
    {
        value = I
    };
};

template <class T>
struct type_to_type
{
    using real_type = T;
    template <class Arg>
    explicit type_to_type(Arg&& arg):
        value(std::forward<Arg>(arg))
    {
    }
    T value;
};

template <bool B>
struct meta_branch
{
    static const bool value = B;
};

template< class T >
struct is_pair
{
    static const bool value = false;
};

template< class T1, class T2 >
struct is_pair< std::pair< T1, T2 > >
{
    static const bool value = true;
};

} // !namespace mpl

} // !namespace vee

#endif // !_VEE_MPL_H_
