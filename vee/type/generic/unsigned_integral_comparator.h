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
struct eqside_type_helper
{
    
};

template <typename LhsTy, typename RhsTy>
struct eqside_type_helper<LhsTy, RhsTy, true, true>
{
    using type = eq_all_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct eqside_type_helper<LhsTy, RhsTy, true, false>
{
    using type = eq_lhs_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct eqside_type_helper<LhsTy, RhsTy, false, true>
{
    using type = eq_rhs_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct eqside_type_helper<LhsTy, RhsTy, false, false>
{
    using type = eq_not_fundamental;
};

template <typename LhsTy, typename RhsTy>
struct eqtype_helper
{
    using lhs_t = LhsTy;
    using rhs_t = RhsTy;
    static constexpr bool lhs_primitive = std::is_fundamental<LhsTy>::value;
    static constexpr bool rhs_primitive = std::is_fundamental<RhsTy>::value;
    using eqs_type_t = typename eqside_type_helper<LhsTy, RhsTy, lhs_primitive, rhs_primitive>::type;
    using eqs_size_t = typename std::conditional< sizeof(lhs_t) == sizeof(rhs_t), same_size_type, typename std::conditional< (sizeof(lhs_t) > sizeof(rhs_t)), lhs_is_bigger, rhs_is_bigger >::type > ::type;
};

} // !namespace detail

template <typename LhsTy, typename RhsTy, typename BaseIntTy>
class unsigned_integral_comparator
{
public:
    using lhs_t = LhsTy;
    using rhs_t = RhsTy;
    using base_int_t = BaseIntTy;
    using eqtype = detail::eqtype_helper<LhsTy, RhsTy>;

    // ctor
    unsigned_integral_comparator(lhs_t& lhs_ref, rhs_t& rhs_ref);
    // dtor
    ~unsigned_integral_comparator() = default;

    bool greater_than();

    const LhsTy& lhs;
    const RhsTy& rhs;
};

template <typename LhsTy, typename RhsTy, typename BaseIntTy>
unsigned_integral_comparator<LhsTy, RhsTy, BaseIntTy>::unsigned_integral_comparator(lhs_t& lhs_ref, rhs_t& rhs_ref):
    lhs{ lhs_ref },
    rhs{ rhs_ref }
{
    // nothing to do.
}

} // !namespace vee

#endif // !_VEE_TYPE_GENERIC_INTEGRAL_COMPARATOR_H_