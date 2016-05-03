#ifndef _VEE_IO_IO_BASE_H_
#define _VEE_IO_IO_BASE_H_

#include <cstdio>
#include <cstdint>

namespace vee {

extern FILE* base_in;
extern FILE* base_out;
extern FILE* base_err;

enum class io_issue: uint32_t
{
    null,
    eof = 1,
};

struct io_result
{
    bool     is_success = false;
    io_issue issue = io_issue::null;
    size_t   bytes_transferred = 0;
};


} // !namespace vee

#endif // !_VEE_IO_IO_BASE_H_
