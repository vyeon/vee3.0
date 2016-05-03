#include <vee/test/testobj.h>
#include <vee/io/io_base.h>

namespace vee {

namespace test {

::std::atomic<int> testobj::_counter;

testobj::testobj():
value(0),
id(_counter.fetch_add(1))
{
    fprintf(base_out, "%s(id: %d)\n", __FUNCSIG__, id);
}

testobj::testobj(int i):
value(i),
id(_counter.fetch_add(1))
{
    fprintf(base_out, "%s(id: %d)\n", __FUNCSIG__, id);
}

testobj::testobj(const testobj& other):
value(other.value),
id(_counter.fetch_add(1))
{
    fprintf(base_out, "%s(id: %d)\n", __FUNCSIG__, id);
}

testobj::testobj(testobj&& other):
value(static_cast<int&&>(other.value)),
id(_counter.fetch_add(1))
{
    fprintf(base_out, "%s(id: %d)\n", __FUNCSIG__, id);
}

testobj::~testobj()
{
    fprintf(base_out, "%s(id: %d)\n", __FUNCSIG__, id);
}

testobj& testobj::operator=(const testobj& other)
{
    value = other.value;
    fprintf(base_out, "%s(id: %d)\n", __FUNCSIG__, id);
    return *this;
}

testobj& testobj::operator=(testobj&& other)
{
    value = static_cast<int&&>(other.value);
    fprintf(base_out, "%s(id: %d)\n", __FUNCSIG__, id);
    return *this;
}

scope::scope(int count):
cnt{ count }
{
    for (int i = 0; i < cnt; ++i)
        printf("-");
    printf("\n");
}

scope::~scope()
{
    for (int i = 0; i < cnt; ++i)
        printf("-");
    printf("\n");
}

} // !namespace test

} // !namespace vee