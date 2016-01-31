#ifndef _VEE_MPL_H_
#define _VEE_MPL_H_

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

template <bool B>
struct meta_branch
{
    static const bool value = B;
};

} // !namespace mpl

} // !namespace vee

#endif // !_VEE_MPL_H_
