#ifndef _VEE_TEST_TIMEREC_H_
#define _VEE_TEST_TIMEREC_H_

#include <chrono>

namespace vee {

namespace test {

class timerec
{
public:
	timerec();
	~timerec();
	::std::pair<time_t, double> timelab() const;
private:
	::std::chrono::time_point<std::chrono::system_clock> start;
};
	
} // !namespace test

} // !namespace vee

#endif // !_VEE_TEST_TIMEREC_H_