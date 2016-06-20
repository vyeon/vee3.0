#ifndef _VEE_MPMATH_H_
#define _VEE_MPMATH_H_

#include <vee/platform.h>

namespace vee {

namespace mpl {

template <class LHS, class RHS>
constexpr inline auto sum(LHS&& lhs, RHS&& rhs) -> decltype(lhs + rhs)
{
    return lhs + rhs;
}

template <class Current, class ...Remainder>
constexpr inline auto sum(Current current, Remainder... rest) -> decltype(current + sum(rest ...))
{
    return (current + sum(rest ...));
}

} // !namespace mpl

} // !namespace vee

#endif // !_VEE_MPMATH_H_
