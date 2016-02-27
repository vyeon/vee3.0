#include <vee/worker.h>
#include <vee/test/testobj.h>
#include <vee/test/timerec.h>
#include <conio.h>

#define FUNC_SIG void(::vee::test::testobj& arg)

::std::mutex cout_lock;

void foo(::vee::test::testobj& arg)
{
    //::std::this_thread::sleep_for(::std::chrono::milliseconds::duration(2000));
    ::std::lock_guard<::std::mutex> locker(cout_lock);
    printf("Call the function foo with arg id(%d) val(%d)\n", arg.id, arg.value);
}

::vee::nonscalable_worker_group<FUNC_SIG> worker_group{ ::std::thread::hardware_concurrency(), 10 };

int main()
{
    ::vee::test::scope scope;
    //worker_group.request(::std::make_shared<::vee::packaged_task<FUNC_SIG>>(::vee::delegate<FUNC_SIG>{foo}, vee::test::testobj{ 1000 }));
    //worker_group.request(::std::make_shared<::vee::packaged_task<FUNC_SIG>>(::vee::delegate<FUNC_SIG>{foo}, vee::test::testobj{ 2000 }));
    //worker_group.request(::std::make_shared<::vee::packaged_task<FUNC_SIG>>(::vee::delegate<FUNC_SIG>{foo}, vee::test::testobj{ 3000 }));
    //::vee::make_delegate<FUNC_SIG>(foo);
    //::vee::packaged_task<FUNC_SIG> task{ ::vee::make_delegate<FUNC_SIG>(foo), vee::test::testobj{ 55 } };
    //::vee::packaged_task<FUNC_SIG> task{ ::vee::delegate<FUNC_SIG>(foo), vee::test::testobj{ 55 } };
    //worker.request(task);
    puts("Press any key to exit...");
    getch();
    return 0;
}