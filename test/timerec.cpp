#include <vee/test/timerec.h>

namespace vee {
	
namespace test {


timerec::timerec()
{
	start = ::std::chrono::system_clock::now();
}

timerec::~timerec()
{
	
}

::std::pair<time_t, double> timerec::timelab()
{
	::std::chrono::time_point<::std::chrono::system_clock> end = ::std::chrono::system_clock::now();
	::std::chrono::duration<double> elapsed_seconds = end - start;
	time_t end_time = ::std::chrono::system_clock::to_time_t(end);
	return ::std::make_pair(end_time, elapsed_seconds.count());
}

} // !namespace test

} // !namespace vee