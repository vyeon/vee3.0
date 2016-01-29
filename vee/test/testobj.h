#ifndef _VEE_TEST_TESTOBJ_H_
#define _VEE_TEST_TESTOBJ_H_

#include <atomic>

namespace vee {

namespace test {

class testobj
{
    static ::std::atomic<int> _counter;
public:
    testobj();
    explicit testobj(int i);
    virtual ~testobj();
    testobj(const testobj&);
    testobj(testobj&&);
    testobj& operator=(const testobj&);
    testobj& operator=(testobj&&);
    int value;
    const int id;
};

} // !namespace test

} // !namespace vee

#endif // !_VEE_TEST_TESTOBJ_H_
