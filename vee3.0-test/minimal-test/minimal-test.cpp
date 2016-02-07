#include <vee/test/testobj.h>
#include <vee/delegate.h>
#include <vee/lockfree/queue.h>
#include <thread>

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
		e(1);
	}
	catch (::vee::exception& ex)
	{
		printf("exception occured! detail: %s\n", ex.to_string());
	}
}

void test_queue()
{
	using namespace std;
	using namespace vee;
	const int test_boundary = INT_MAX / 256;
	lockfree::queue<int> queue{ 1000 };	
	::std::atomic<int> produced = 0;
	auto producer = [&]()->void
	{
		int produced_local = 0;
		for (int i = 0; produced.load() < test_boundary; ++i)
		{
			if (queue.enqueue(i))
			{
				++produced;
				++produced_local;
			}
		}
		printf("Produced %d times\n", produced_local);
	};
	thread producer1{producer};
	thread producer2{producer};

	::std::atomic<int> consumed = 0;

	auto consumer = [&]()->void
	{
		int consumed_local = 0;
		while (true)
		{
			int r;
			if (queue.dequeue(r))
			{
				++consumed;
				++consumed_local;
			}
			else
			{
				//printf("dequeue failed!\n");
			}
			if (consumed > test_boundary)
				break;
		}
		printf("Consumed %d times\n", consumed_local);
	};

	thread consumer1(consumer);
	thread consumer2(consumer);
	thread consumer3(consumer);
	if (consumer1.joinable())
		consumer1.join();
	if (consumer2.joinable())
		consumer2.join();
	if (consumer3.joinable())
		consumer3.join();
	if (producer1.joinable()) 
		producer1.join();
	if (producer2.joinable())
		producer2.join();
}

int main()
{
    //test_delegate();
	test_queue();
    return 0;
}