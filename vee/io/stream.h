#ifndef _VEE_IO_STREAM_H_
#define _VEE_IO_STREAM_H_

#include <vee/delegate.h>
#include <vee/io/io_base.h>
#include <vee/io/io_service.h>

namespace vee {

namespace io {

using buffer_t = uint8_t*;

struct async_input_info
{
    using shared_ptr = ::std::shared_ptr<async_input_info>;
    io_result result;
    buffer_t  buffer;
    size_t    capacity;
    async_input_info(buffer_t __buffer, size_t __capacity):
        buffer { __buffer },
        capacity { __capacity }
    {
        
    }
    ~async_input_info()
    {
        
    }
private:
    async_input_info() = delete;
};

struct async_output_info
{
    using shared_ptr = ::std::shared_ptr<async_output_info>;
    io_result result;
    buffer_t  buffer;
    size_t    requested_size;
    size_t    capacity;
    async_output_info(buffer_t __buffer, size_t __requested_size, size_t __capacity):
        buffer{ __buffer },
        requested_size { __requested_size },
        capacity{ __capacity }
    {

    }
    ~async_output_info()
    {
        
    }
private:
    async_output_info() = delete;
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
    virtual size_t write_some(const uint8_t* buffer, const size_t size) = 0;
    virtual size_t read_some(uint8_t* const buffer, const size_t size) = 0;
};

class async_stream abstract: virtual public stream_base
{
public:
    using this_t = async_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
    using async_read_delegate  = delegate<void(io::async_input_info::shared_ptr), lock::spin_lock>;
    using async_write_delegate = delegate<void(io::async_output_info::shared_ptr), lock::spin_lock>;
    virtual ~async_stream() = default;
    virtual void async_read_some(io::async_input_info::shared_ptr info, async_read_delegate::shared_ptr callback) __noexcept = 0;
    virtual void async_write_some(io::async_output_info::shared_ptr info, async_write_delegate::shared_ptr callback) __noexcept = 0;
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