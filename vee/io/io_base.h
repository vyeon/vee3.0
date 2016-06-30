#ifndef _VEE_IO_IO_BASE_H_
#define _VEE_IO_IO_BASE_H_

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

class stream_base;
class sync_stream;
class async_stream;
class io_stream;

struct async_io_result;
struct async_result;
using async_io_delegate = delegate<void(async_io_result&)>;
using async_io_callback = async_io_delegate::shared_ptr;

template <class Result, class ...FwdArgs>
inline Result async_callback(FwdArgs&& ...args)
{
    return ::std::make_shared< ::std::pointer_traits<Result>::element_type >(::std::forward<FwdArgs>(args)...);
}

struct async_result
{
    bool is_success = false;
};

struct async_io_result: public async_result
{
    io_stream* stream_ptr { nullptr };
    io_issue   issue = io_issue::null;
    size_t     bytes_transferred = 0;
    size_t     bytes_requested = 0;
    io::buffer buffer {};
    async_io_callback callback { nullptr };
};

} // !namespace vee

#endif // !_VEE_IO_IO_BASE_H_
