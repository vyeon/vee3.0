#ifndef _VEE_NET_H_
#define _VEE_NET_H_

#include <vee/io/stream.h>
#include <vee/platform.h>

namespace vee {

namespace net {

class protocol_mismatch_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    protocol_mismatch_exception():
        base_t{ "protocol mismatch" }
    {
    }
    virtual ~protocol_mismatch_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class connection_failed_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    connection_failed_exception():
        base_t{ "connection failed" }
    {
    }
    explicit connection_failed_exception(char const* const);
    virtual ~connection_failed_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class connection_already_disconnected: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    connection_already_disconnected():
        base_t{ "connection already disconnected" }
    {
    }
    virtual ~connection_already_disconnected() = default;
    virtual char const* to_string() const noexcept override;
};

class accept_failed_exception: virtual public ::vee::exception
{
public:
    using base_t = ::vee::exception;
    accept_failed_exception():
        base_t{ "accept failed exception" }
    {
    }
    virtual ~accept_failed_exception() = default;
    virtual char const* to_string() const noexcept override;
};

namespace ip {

using port_t = unsigned short;
#if VEE_PLATFORM_X32
using socketfd_t = uint32_t;
#elif VEE_PLATFORM_X64
using socketfd_t = uint64_t;
#endif // VEE_PLATFORM

// Forward declaration
class socket_stream;
class connectable_stream;
struct async_connect_result;
using socket_stream_handle = std::shared_ptr<socket_stream>;
using connectable_stream_handle = std::shared_ptr<connectable_stream>;

using async_connect_delegate = delegate<void(async_connect_result&)>;
//using async_connect_callback = async_connect_delegate::shared_ptr;
using async_connect_callback = std::function<void(async_connect_result&)>;

struct async_connect_result : public async_result
{
    using this_t = async_connect_result;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<async_connect_result>;
    connectable_stream_handle stream;
    std::string ip;
    port_t port;
    async_connect_callback callback;
    std::string message;
};

class socket_stream abstract : virtual public io_stream
{
public:
    using this_t = socket_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual socketfd_t native() noexcept = 0;
};

class connectable_stream abstract : virtual public socket_stream
{
public:
    using this_t = connectable_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~connectable_stream() noexcept = default;
    virtual void connect(const char* ip, port_t port) = 0;
    virtual void disconnect() = 0;
    virtual void async_connect(const char* ip, port_t port, async_connect_callback callback) noexcept = 0;
    virtual void async_connect(const char* ip, port_t port, async_connect_delegate::shared_ptr callback) noexcept = 0;
    virtual bool is_open() noexcept = 0;

    delegate<void()> on_destroy;
};

namespace tcp {

struct async_accept_result;

using async_accept_delegate = delegate<void(async_accept_result&)>;
//using async_accept_callback = async_accept_delegate::shared_ptr;
using async_accept_callback = std::function<void(async_accept_result&)>;

class server abstract
{
public:
    using this_t = server;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~server() noexcept = default;
    virtual void close() noexcept = 0;
    virtual connectable_stream_handle accept() = 0;
    virtual void async_accept(async_accept_callback callback) = 0;
    virtual void async_accept(async_accept_delegate::shared_ptr callback) = 0;
    virtual io_service& get_io_service() noexcept = 0;
};

using server_t = server::shared_ptr;

struct async_accept_result : public async_result
{
    using shared_ptr = std::shared_ptr<async_accept_result>;
    server* server_ptr{ nullptr };
    connectable_stream_handle session;
    std::string message;
    async_accept_callback callback;
};

connectable_stream_handle create_session(io_service& iosvc) noexcept;
server_t create_server(io_service& iosvc, port_t port) noexcept;

} // !namespace tcp

namespace udp {

    socket_stream_handle create_stream(io_service& iosvc) noexcept;

} // !namespace udp

namespace rfc6455 {

    connectable_stream_handle create_session(io_service& iosvc) noexcept;
    tcp::server::shared_ptr create_server(io_service& iosvc, port_t port) noexcept;

} // !namespace rfc6455

} // !namespace ip

} // !namespace net

} // !namespace vee

#endif // !_VEE_NET_H_