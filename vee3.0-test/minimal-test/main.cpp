#include <vee/worker.h>
#include <vee/test/testobj.h>
#include <vee/test/timerec.h>

#define FUNC_SIG void(::vee::test::testobj& arg)

::std::mutex cout_lock;

void foo(::vee::test::testobj& arg)
{
    ::std::lock_guard<::std::mutex> locker(cout_lock);
    printf("Call the function foo with arg id(%d) val(%d)\n", arg.id, arg.value);
}

int main()
{
    ::vee::test::scope scope;
    ::vee::worker<FUNC_SIG> worker{ 10 };
    //::vee::make_delegate<FUNC_SIG>(foo);
    ::vee::packaged_task<FUNC_SIG> task{ ::vee::make_delegate<FUNC_SIG>(foo), vee::test::testobj{ 55 } };
    //::vee::packaged_task<FUNC_SIG> task{ ::vee::delegate<FUNC_SIG>(foo), vee::test::testobj{ 55 } };
    worker.request(task);
    worker.request(task);
    worker.request(task);
    worker.request(task);
    worker.request(task);
    worker.request(task);
    worker.request(task);
    worker.request(task);
    return 0;
}