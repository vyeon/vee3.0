#ifndef _VEE_TYPE_GENERIC_INTEGRAL_COMPARATOR_H_
#define _VEE_TYPE_GENERIC_INTEGRAL_COMPARATOR_H_
#include <cstdint>
#include <vee/mpl.h>

namespace vee {

namespace detail {

VEE_DECLARE_TYPE(eq_lhs_fundamental);
VEE_DECLARE_TYPE(eq_rhs_fundamental);
VEE_DECLARE_TYPE(eq_all_fundamental);
VEE_DECLARE_TYPE(eq_not_fundamental);

VEE_DECLARE_TYPE(lhs_is_bigger);
VEE_DECLARE_TYPE(rhs_is_bigger);
VEE_DECLARE_TYPE(same_size_type);

template <typename LhsTy, typename RhsTy, bool /*LeftIsFundamental*/, bool /*RightIsFundamental*/>
struct equation_side_type
{
    
};

template <typename LhsTy, typename RhsTy>
struct equation_side_type<LhsTy, RhsTy, true, true>
{
    using type = eq_all_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct equation_side_type<LhsTy, RhsTy, true, false>
{
    using type = eq_lhs_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct equation_side_type<LhsTy, RhsTy, false, true>
{
    using type = eq_rhs_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct equation_side_type<LhsTy, RhsTy, false, false>
{
    using type = eq_not_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct equation_type_helper
{
    using lhs_t = LhsTy;
    using rhs_t = RhsTy;
    static constexpr bool lhs_is_fundamental = std::is_fundamental<LhsTy>::value;
    static constexpr bool rhs_is_fundamental = std::is_fundamental<RhsTy>::value;
    using eqs_type_t = typename equation_side_type<LhsTy, RhsTy, lhs_is_fundamental, rhs_is_fundamental>::type;
    using eqs_size_t = typename std::conditional< sizeof(lhs_t) == sizeof(rhs_t), same_size_type, typename std::conditional< (sizeof(lhs_t) > sizeof(rhs_t)), lhs_is_bigger, rhs_is_bigger >::type > ::type;
};

} // !namespace detail

template <typename LhsTy, typename RhsTy, typename BaseIntTy = uint64_t>
class unsigned_integral_comparator
{
public:
    using lhs_t = LhsTy;
    using rhs_t = RhsTy;
    using base_int_t = BaseIntTy;

    // ctor
    unsigned_integral_comparator(lhs_t& lhs_ref, rhs_t& rhs_ref);
    // dtor
    ~unsigned_integral_comparator();

    const LhsTy& lhs;
    const RhsTy& rhs;
};

} // !namespace vee

#endif // !_VEE_TYPE_GENERIC_INTEGRAL_COMPARATOR_H_