#include <vee/libtest.h>
#include <vee/test/testobj.h>
#include <vee/type/generic/unsigned_integer.h>
#include <limits>

namespace vee {

namespace libtest {

namespace {

template <int TypeSize>
size_t test_sizeof_operator()
{
    bool result = (sizeof(unsigned_integer<TypeSize>) == TypeSize);
    test_log(result, __FUNCTION__, "test type: unsigned_integer<%d>", TypeSize);
    return (result) ? 1 : 0;
}

template <int TypeSize>
size_t test_initalize_and_equal_with_uint32_t(uint32_t init, uint32_t base)
{
    unsigned_integer<TypeSize> i{ init };
    bool result = ((i == base) == (init == base));
    test_log(result, __FUNCTION__, "test type: unsigned_integer<%d>, initial value: %u, base: %u", TypeSize, init, base);
    return (result) ? 1 : 0;
}

template <int LhsTypeSize, int RhsTypeSize>
size_t test_initialize_and_equal(uint32_t lhs_init, uint32_t rhs_init)
{
    unsigned_integer<LhsTypeSize> lhs{ lhs_init };
    unsigned_integer<RhsTypeSize> rhs{ rhs_init };
    bool result = ((lhs == rhs) == (lhs_init == rhs_init));
    test_log(result, __FUNCTION__, "LHS: unsigned_integer<%d>{%u}, RHS: unsigned_integer<%d>{%u}", LhsTypeSize, lhs_init, RhsTypeSize, rhs_init);
    return (result) ? 1 : 0;
}

template <int TypeSize>
size_t test_greater_than_with_uint32_t(uint32_t init, uint32_t base)
{
    unsigned_integer<TypeSize> i{ init };
    bool result = ((i > base) == (init > base));
    test_log(result, __FUNCTION__, "test type: unsigned_integer<%d>, initial value: %u, base: %u", TypeSize, init, base);
    return (result) ? 1 : 0;
}

template <int LhsTypeSize, int RhsTypeSize>
size_t test_greater_than(uint32_t lhs_init, uint32_t rhs_init)
{
    unsigned_integer<LhsTypeSize> lhs{ lhs_init };
    unsigned_integer<RhsTypeSize> rhs{ rhs_init };
    bool result = ((lhs > rhs) == (lhs_init > rhs_init));
    test_log(result, __FUNCTION__, "LHS: unsigned_integer<%d>{%u}, RHS: unsigned_integer<%d>{%u}", LhsTypeSize, lhs_init, RhsTypeSize, rhs_init);
    return (result) ? 1 : 0;
}

}; // !unnamed namespace

size_t test_type_generic::test_all() noexcept
{
    test::scope scope;
    size_t error = 0;
    error += test_sizeof_operator<3>();
    error += test_sizeof_operator<5>();
    error += test_sizeof_operator<128>();
    error += test_initalize_and_equal_with_uint32_t<4>(16, 16);
    error += test_initalize_and_equal_with_uint32_t<4>(17, 16);
    error += test_initalize_and_equal_with_uint32_t<2>(16, 16);
    error += test_initalize_and_equal_with_uint32_t<2>(17, 16);
    error += test_initalize_and_equal_with_uint32_t<8>(16, 16);
    error += test_initalize_and_equal_with_uint32_t<8>(17, 16);
    error += test_initialize_and_equal<4, 4>(16, 16);
    error += test_initialize_and_equal<4, 4>(17, 16);
    error += test_initialize_and_equal<4, 8>(16, 16);
    error += test_initialize_and_equal<4, 8>(17, 16);
    error += test_initialize_and_equal<8, 4>(16, 16);
    error += test_initialize_and_equal<8, 4>(17, 16);
    error += test_greater_than_with_uint32_t<4>(16, 16);
    error += test_greater_than_with_uint32_t<4>(17, 16);
    error += test_greater_than_with_uint32_t<2>(16, 16);
    error += test_greater_than_with_uint32_t<2>(17, 16);
    error += test_greater_than_with_uint32_t<8>(16, 16);
    error += test_greater_than_with_uint32_t<8>(17, 16);
    error += test_greater_than<4, 16>(16, 16);
    error += test_greater_than<4, 16>(17, 16);
    error += test_greater_than<4, 16>(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max() - 1);
    error += test_greater_than<4, 16>(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max() - std::numeric_limits<int32_t>::max());
    error += test_greater_than<4, 16>(0, std::numeric_limits<uint32_t>::max() - std::numeric_limits<int32_t>::max());

    return error;
}

} // !namespace libtest

} // !namespace vee