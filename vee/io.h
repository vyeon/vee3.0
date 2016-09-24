#ifndef _VEE_IO_STREAM_H_
#define _VEE_IO_STREAM_H_

#include <vee/io/port_base.h>
#include <vee/io/io_service.h>

namespace vee {

class invalid_stream_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    invalid_stream_exception():
        base_t{ "invalid stream exception" }
    {
    }
    explicit invalid_stream_exception(char const* const);
    virtual ~invalid_stream_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class stream_write_failed_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    stream_write_failed_exception():
        base_t{ "stream write failed exception" }
    {
    }
    explicit stream_write_failed_exception(char const* const);
    virtual ~stream_write_failed_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class stream_reset_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    stream_reset_exception():
        base_t{ "stream corrupted exception" }
    {
    }
    explicit stream_reset_exception(char const* const);
    virtual ~stream_reset_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class unknown_io_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    unknown_io_exception():
        base_t{ "unknown io exception" }
    {
    }
    explicit unknown_io_exception(char const* const);
    virtual ~unknown_io_exception() = default;
    virtual char const* to_string() const noexcept override;
};

namespace io {

class sync_port: virtual public port_base
{
public:
    using this_t = sync_port;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~sync_port() = default;
    virtual size_t write_some(const io::buffer& buffer, const size_t bytes_requested) = 0;
    virtual size_t read_explicit(io::buffer buffer, const size_t bytes_requested) = 0;
    virtual size_t read_some(io::buffer buffer, size_t maximum_read_bytes) = 0;
};

class async_port: virtual public port_base
{
public:
    using this_t = async_port;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~async_port() = default;
    // I/O member functions of std::function type callback
    virtual void async_read_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept = 0;
    virtual void async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept = 0;
    virtual void async_write_some(const io::buffer& buffer, size_t bytes_requested, async_io_callback callback) noexcept = 0;
    // I/O member functions of delegate
    virtual void async_read_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept = 0;
    virtual void async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept = 0;
    virtual void async_write_some(const io::buffer& buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept = 0;
    virtual io::io_service& get_io_service() const noexcept = 0;
};

class io_port: public sync_port, public async_port
{
public:
    using this_t = io_port;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~io_port() = default;
};

} // !namespace io

} // !namespace vee

#endif // !_VEE_IO_STREAM_H_