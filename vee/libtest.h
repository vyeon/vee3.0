#ifndef _VEE_LIBTEST_H_
#define _VEE_LIBTEST_H_

namespace vee {

namespace libtest {

void test_log(bool result, const char* test_name, const char* detail, ...) noexcept;
void test_success(const char* test_name, const char* detail) noexcept;
void test_failed(const char* test_name, const char* detail) noexcept;

class test_interface
{
public:
    virtual ~test_interface() = default;
    // returns the number of errors
    virtual size_t test_all() noexcept = 0;
};

#define DECLARE_TEST_CLASS(test_class_name) \
class test_class_name: public test_interface\
{\
public:\
    virtual ~test_class_name() = default;\
    virtual size_t test_all() noexcept override;\
};

DECLARE_TEST_CLASS(test_type_generic);

#undef DECLARE_TEST_CLASS

} // !namespace libtest

} // !namespace vee

#endif // !_VEE_LIBTEST_H_