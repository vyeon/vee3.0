#include <vee/test/testobj.h>
#include <vee/delegate.h>
#include <vee/lockfree/stack.h>
#include <vee/test/timerec.h>
#include <vee/queue.h>
#include <thread>
#include <vector>
#include <mutex>
#include <iostream>
#include <stack>

#pragma warning(default:4127)

#define FUNCSIG void(int)

::std::mutex log_mutex;

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

	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	//queue<size_t, mutex> queue { 10000 };
	lockfree::queue<size_t> queue{ 100000 };
	const size_t produce_per_thread = 5000000;
	const size_t number_of_procducers = 5;
	const size_t number_of_consumers = 2;
	
	auto producer = [&]() -> void
	{
		{		
			::std::lock_guard<::std::mutex> locker{ log_mutex };
			cout << "producer " << this_thread::get_id() << " start." << endl;
		}
		size_t produced = 0;
		size_t tries = 0;
		size_t fails = 0;
		for (size_t i = 0; produced != produce_per_thread; ++i)
		{
			++tries;
			if(queue.enqueue(i))
			{
				++produced;
			}
			else
			{
				++fails;
			}
		}
		double ratio = (1.0 - static_cast<double>(fails) / static_cast<double>(tries)) * 100.0;
		{
			::std::lock_guard<::std::mutex> locker{ log_mutex };
			cout << "producer " << this_thread::get_id() << " is done." << endl
				<< tries << " times tried, "
				<< fails << " times failed ("
				<< ratio << "%)" << endl;
		}
		return;
	};	

	auto consumer = [&]() -> void
	{
		static atomic<size_t> consumed_all = 0;
		{
			::std::lock_guard<::std::mutex> locker{ log_mutex };
			cout << "consumer " << this_thread::get_id() << " start." << endl;
		}
		size_t consumed = 0;
		size_t tries = 0;
		size_t fails = 0;
		while (true)
		{
			size_t val;
			++tries;
			if (queue.dequeue(val))
			{
				++consumed;
				++consumed_all;
			}
			else
			{
				++fails;
			}
			if (consumed_all.load() == number_of_procducers * produce_per_thread)
				break;
		}
		double ratio = (1.0 - static_cast<double>(fails) / static_cast<double>(tries)) * 100.0;
		{
			::std::lock_guard<::std::mutex> locker{ log_mutex };
			cout << "consumer " << this_thread::get_id() << " is done." << endl
				<< tries << " times tried, "
				<< fails << " times failed ("
				<< ratio << "%)" << endl;
		}
		return;
	};

	vector<thread> threads;
	for (int i = 0; i < number_of_procducers; ++i)
		threads.push_back(thread{producer});
	for (int i = 0; i < number_of_consumers; ++i)
		threads.push_back(thread{consumer});
	for (auto& thr : threads)
		if(thr.joinable()) 
			thr.join();

	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	time_t end_time = chrono::system_clock::to_time_t(end);

	cout << "finished computation at " << ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";
}

void test_stack()
{
	using namespace std;
	using namespace vee;
	lockfree::stack<int> stack{ 20 };
	for (int i = 0; i < 30; ++i)
	{
		printf("push %d : %s\n", i, stack.push(i)?"success":"failed");
	}
	for (int i = 0; i < 30; ++i)
	{
		int out;
		if(stack.pop(out))
		{
			printf("pop success, val: %d\n", out);
		}
		else
		{
			printf("pop failed\n");
		}
	}
}

void lockfree_queue_input()
{
	static ::vee::lockfree::queue<int> lfqueue{ 100000 };
	for (int i = 0; i < 5000000; ++i)
	{
		lfqueue.enqueue(i);
	}
}

void lock_queue_input()
{
	static ::vee::queue<int, ::std::mutex, ::std::mutex> lqueue{ 100000 };
	for (int i = 0; i < 5000000; ++i)
	{
		lqueue.enqueue(i);
	}
}

void test()
{
	using namespace vee;
	{
		test::scope sc;
		printf("Locked queue (::std::mutex)\n");
		test::timerec tr;
		::std::vector<std::thread> thrs;
		for (auto i = 0; i < 20; ++i)
			thrs.push_back(::std::thread(lock_queue_input));
		for(auto& thr : thrs)
		{
			if(thr.joinable())
				thr.join();
		}
		auto result = tr.timelab();
		::std::cout << "finished computation at " << ctime(&result.first)
			<< "elapsed time: " << result.second << "s\n";
	}

	{
		test::scope sc;
		printf("Lockfree queue\n");
		test::timerec tr;
		::std::vector<std::thread> thrs;
		for (auto i = 0; i < 20; ++i)
			thrs.push_back(::std::thread(lockfree_queue_input));
		for (auto& thr : thrs)
		{
			if (thr.joinable())
				thr.join();
		}
		auto result = tr.timelab();
		::std::cout << "finished computation at " << ctime(&result.first)
			<< "elapsed time: " << result.second << "s\n";
	}
	return;
}

int main()
{
    //test_delegate();
	//test_queue();
	//test_stack();
	test();
    return 0;
}
