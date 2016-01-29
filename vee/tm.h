#ifndef _VEE_TM_H_
#define _VEE_TM_H_

namespace vee {

namespace tm {

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

} // !namespace tm

} // !namespace vee

#endif // !_VEE_TM_H_
