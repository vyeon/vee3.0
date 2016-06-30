#ifndef _VEE_IO_STREAM_H_
#define _VEE_IO_STREAM_H_

#include <vee/io/io_base.h>
#include <vee/io/io_service.h>

namespace vee {

namespace io {

class invalid_stream_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    invalid_stream_exception():
        base_t{ "invalid stream exception" }
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
        base_t{ "stream write failed exception" }
    {
    }
    explicit stream_write_failed_exception(char const* const);
    virtual ~stream_write_failed_exception() = default;
    virtual char const* to_string() const __noexcept override;
};

class stream_reset_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    stream_reset_exception():
        base_t{ "stream corrupted exception" }
    {
    }
    explicit stream_reset_exception(char const* const);
    virtual ~stream_reset_exception() = default;
    virtual char const* to_string() const __noexcept override;
};

class unknown_io_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    unknown_io_exception():
        base_t{ "unknown io exception" }
    {
    }
    explicit unknown_io_exception(char const* const);
    virtual ~unknown_io_exception() = default;
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
    virtual size_t write_some(io::buffer buffer, const size_t bytes_requested) = 0;
    virtual size_t read_explicit(io::buffer buffer, const size_t bytes_requested) = 0;
    virtual size_t read_some(io::buffer buffer, size_t maximum_read_bytes) = 0;
};

class async_stream abstract: virtual public stream_base
{
public:
    using this_t = async_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
    virtual ~async_stream() = default;
    virtual void async_read_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept = 0;
    virtual void async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept = 0;
    virtual void async_write_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept = 0;
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