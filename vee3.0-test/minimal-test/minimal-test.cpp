#include <vee/test/testobj.h>
#include <vee/delegate.h>

#define FUNCSIG void(int)

void foo(int i)
{
	printf("%s(int i = %d)\n", __FUNCTION__, i);
}

void bar(int i)
{
	printf("%s(int i = %d)\n", __FUNCTION__, i);
}

void test_delegate()
{
	using namespace vee;
	delegate<FUNCSIG> e;
	try
	{
		e += ::std::make_pair(100, foo);
		e += ::std::make_pair(100, bar);
	}
	catch (::vee::exception& ex)
	{
		printf("exception occured! detail: %s\n", ex.to_string());
	}
}

int main()
{
    test_delegate();
    return 0;
}