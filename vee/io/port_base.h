#ifndef _VEE_IOPORT_IOPORT_BASE_H_
#define _VEE_IOPORT_IOPORT_BASE_H_

#include <vee/delegate.h>
#include <vee/io/io_service.h>

namespace vee {

namespace io {

extern FILE* base_in;
extern FILE* base_out;
extern FILE* base_err;

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

class port_base
{
public:
    using this_t = port_base;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~port_base() noexcept;
    delegate<void()> on_destroy;
};

class sync_port;
class async_port;
class io_port;
struct async_io_result;
struct async_result;
using async_io_delegate = delegate<void(async_io_result&)>;
using async_io_callback = std::function<void(async_io_result&)>;

enum class io_issue: uint32_t
{
    null,
    unknown = 1,
    eof,
    connection_reset,
};

template <class Result, class ...FwdArgs>
inline Result async_delegate_callbck(FwdArgs&& ...args)
{
    return std::make_shared<typename std::pointer_traits<Result>::element_type >(std::forward<FwdArgs>(args)...);
}

struct async_result
{
    bool is_success = false;
};

struct async_io_result: public io::async_result
{
    async_port* stream_ptr{ nullptr };
    io_issue   issue = io_issue::null;
    size_t     bytes_transferred = 0;
    size_t     bytes_requested = 0;
    io::buffer buffer{};
    async_io_callback callback{ nullptr };
};

struct io_result
{
    bool     is_success = false;
    io_issue issue = io_issue::null;
    size_t   bytes_transferred = 0;
};

} // !namespace io

} // !namespace vee

#endif // !_VEE_IOPORT_IOPORT_BASE_H_
