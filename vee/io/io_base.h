#ifndef _VEE_IO_IO_BASE_H_
#define _VEE_IO_IO_BASE_H_

#include <cstdio>
#include <cstdint>
#include <vee/delegate.h>

namespace vee {

extern FILE* base_in;
extern FILE* base_out;
extern FILE* base_err;

namespace io {

struct buffer
{
    uint8_t* ptr = nullptr;
    size_t   capacity = 0;
    buffer() = default;
    ~buffer() = default;
    buffer(uint8_t* __ptr, size_t __capacity):
        ptr{ __ptr },
        capacity { __capacity }
    {
    }
};

} // !namespace io

enum class io_issue: uint32_t
{
    null,
    unknown = 1,
    eof,
    connection_reset,
};

struct io_result
{
    bool     is_success = false;
    io_issue issue = io_issue::null;
    size_t   bytes_transferred = 0;
};

struct async_io_result;
struct async_result;
using async_io_delegate = delegate<void(async_io_result&)>;

struct async_result
{
    bool is_success = false;
};

struct async_io_result: public async_result
{
    io_issue   issue = io_issue::null;
    size_t     bytes_transferred = 0;
    size_t     bytes_requested = 0;
    io::buffer buffer {};
    async_io_delegate::shared_ptr callback { nullptr };
};


} // !namespace vee

#endif // !_VEE_IO_IO_BASE_H_
