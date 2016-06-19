#ifndef _VEE_IO_STREAM_H_
#define _VEE_IO_STREAM_H_

#include <vee/delegate.h>
#include <vee/io/io_base.h>
#include <vee/io/io_service.h>

namespace vee {

namespace io {

using buffer_t = uint8_t*;
using in_buffer_t = uint8_t* const;  // read
using out_buffer_t = const uint8_t*; // write

struct async_result
{
    using shared_ptr = ::std::shared_ptr<async_result>;
    bool is_success = false;
};

struct async_io_result: public async_result
{
    using shared_ptr = ::std::shared_ptr<async_io_result>;
    io_result result;
    size_t    bytes_transferred;
};

struct async_read_result: public async_io_result
{
    using shared_ptr = ::std::shared_ptr<async_read_result>;
    buffer_t  buffer;
    size_t    capacity;
    async_read_result(buffer_t __buffer, size_t __capacity):
        buffer { __buffer },
        capacity { __capacity }
    {
        
    }
    ~async_read_result()
    {
        
    }
private:
    async_read_result() = delete;
};

struct async_write_result: public async_io_result
{
    using shared_ptr = ::std::shared_ptr<async_write_result>;
    buffer_t  buffer;
    size_t    capacity;
    async_write_result(buffer_t __buffer, size_t __capacity):
        buffer{ __buffer },
        capacity{ __capacity }
    {

    }
    ~async_write_result()
    {
        
    }
private:
    async_write_result() = delete;
};

class invalid_stream_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    invalid_stream_exception():
        base_t{ "invalid stream" }
    {
    }
    explicit invalid_stream_exception(char const* const);
    virtual ~invalid_stream_exception() = default;
    virtual char const* to_string() const __noexcept override;
};

class stream_write_failed_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    stream_write_failed_exception():
        base_t{ "" }
    {
    }
    explicit stream_write_failed_exception(char const* const);
    virtual ~stream_write_failed_exception() = default;
    virtual char const* to_string() const __noexcept override;
};

} // !namespace io

class stream_base abstract
{
public:
    using this_t = stream_base;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
    virtual ~stream_base() = default;
    virtual io_service& get_io_service() __noexcept = 0;
};

class sync_stream abstract: virtual public stream_base
{
public:
    using this_t = sync_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
    virtual ~sync_stream() = default;
    virtual size_t write_some(io::out_buffer_t buffer, const size_t size) = 0;
    virtual size_t read_explicit(io::in_buffer_t buffer, const size_t size) = 0;
    virtual size_t read_some(io::in_buffer_t buffer, const size_t size) = 0;
};

class async_stream abstract: virtual public stream_base
{
public:
    using this_t = async_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
    using async_read_delegate  = delegate<void(io::async_read_result), lock::spin_lock>;
    using async_write_delegate = delegate<void(io::async_write_result), lock::spin_lock>;
    virtual ~async_stream() = default;
    virtual void async_read_some(io::in_buffer_t buffer, size_t capacity, async_read_delegate::shared_ptr callback) __noexcept = 0;
    virtual void async_read_explicit(io::in_buffer_t buffer, size_t bytes_requested, async_read_delegate::shared_ptr callback) __noexcept = 0;
    virtual void async_write_some(io::out_buffer_t buffer, size_t bytes_requested, async_write_delegate::shared_ptr callback) __noexcept = 0;
};

class io_stream abstract: virtual public sync_stream, virtual public async_stream
{
public:
    using this_t = io_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
    virtual ~io_stream() = default;
};

} // !namespace vee

#endif // !_VEE_IO_STREAM_H_