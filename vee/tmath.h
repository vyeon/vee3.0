#ifndef _VEE_TMATH_H_
#define _VEE_TMATH_H_

namespace vee {

template <class LHS, class RHS>
__constexpr inline auto sum(LHS&& lhs, RHS&& rhs) -> decltype(lhs + rhs)
{
    return lhs + rhs;
}

template <class Current, class ...Remainder>
__constexpr inline auto sum(Current current, Remainder... rest) -> decltype(current + sum(rest ...))
{
    return (current + sum(rest ...));
}

} // !namespace vee

#endif // !_VEE_TMATH_H_
