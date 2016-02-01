#include <vee/test/testobj.h>
#include <vee/delegate.h>
using namespace std;

#define FUNCSIG void(int, ::vee::test::testobj&)

void foo(int i, ::vee::test::testobj& obj)
{
    printf("%s (id = %d, testobj = %d)\n", __FUNCTION__, i, obj.id);
}

void bar(int i, ::vee::test::testobj& obj)
{
    printf("%s (id = %d, testobj = %d)\n", __FUNCTION__, i, obj.id);
}

void test_tupleupk()
{
    printf("*********************** TUPLEUPK TEST CODE BEGIN ***********************\n");
    ::vee::tupleupk(foo, make_tuple(1, ::vee::test::testobj(4)));
}

void test_delegate()
{
    printf("*********************** DELEGATE TEST CODE BEGIN ***********************\n");

    {
        ::vee::test::scope scope;
        puts("Default Constructor");
        // Default constructor
        ::vee::delegate<FUNCSIG> e;
    }

    {
        ::vee::test::scope scope;
        puts("Constructor with normal function parameter");
        // Constructor with normal function parameter
        ::vee::delegate<FUNCSIG> e{ foo };
    }

    {
        ::vee::test::scope scope;
        puts("Constructor with binder type (lvalue reference) (standard function class)");
        // Constructor with binder type, lvalue reference (standard function class)
        ::std::function<FUNCSIG> f{ foo };
        ::vee::delegate<FUNCSIG> e{ f };
    }

    {
        ::vee::test::scope scope;
        puts("Constructor with binder type (rvalue reference) (standard function class)");
        // Constructor with binder type, rvalue reference (standard function class)
        ::vee::delegate<FUNCSIG> e{ ::std::function<FUNCSIG>{ foo } };
    }

    {
        ::vee::test::scope scope;
        puts("Constructor with user key and binder pair (lvalue reference)");
        // Constructor with user key and binder pair, lvalue reference
        int key = 100;
        ::std::function<FUNCSIG> binder{ foo };
        ::vee::delegate<FUNCSIG> e{ key, binder };
    }

    {
        ::vee::test::scope scope;
        puts("Constructor with user key and binder pair (rvalue reference)");
        // Constructor with user key and binder pair, rvalue reference
        ::vee::delegate<FUNCSIG> e{ 101, ::std::function<FUNCSIG>{ foo } };
    }

    {
        ::vee::test::scope scope;
        ::vee::delegate<FUNCSIG> e;
        puts("Test the operator +=, (), -=");
        // Test the operator +=
        e += foo; // standard function
        e += ::std::function<FUNCSIG>(bar); // binder
        puts("Call foo and bar");
        e(1, ::vee::test::testobj());
        e -= foo;
        puts("Call bar");
        e(2, ::vee::test::testobj());
    }
}

int main()
{
    test_tupleupk();
    test_delegate();
    return 0;
}