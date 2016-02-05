#ifndef _VEE_IOBASE_H_
#define _VEE_IOBASE_H_

#include <cstdio>

namespace vee {

extern FILE* base_in;
extern FILE* base_out;
extern FILE* base_err;

struct io_result
{
	bool   failed;
	bool   eof;
	size_t bytes_processed;
};

} // !namespace vee

#endif // !_VEE_IOBASE_H_
