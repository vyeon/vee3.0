#include <vee/tupleupk.h>
#include <vee/test/testobj.h>
using namespace std;
using namespace vee;

void foo(int a, int b, float c, test::testobj& obj)
{
    printf("%d %d %.1f %d\n", a, b, c, obj.value);
}

int main()
{
    call_by_tuple(foo, make_tuple(1, 2, (float)3.0, test::testobj(4)));
    return 0;
}