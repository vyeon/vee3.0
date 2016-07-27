#ifndef _VEE_NET_H_
#define _VEE_NET_H_

#include <vee/io/stream.h>
#include <vee/platform.h>

namespace vee {

namespace net {

class protocol_mismatch_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    protocol_mismatch_exception():
        base_t{ "protocol mismatch" }
    {
    }
    virtual ~protocol_mismatch_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class connection_failed_exception: public vee::exception
{
public:
    using base_t = vee::exception;
    connection_failed_exception():
        base_t{ "connection failed" }
    {
    }
    explicit connection_failed_exception(char const* const);
    virtual ~connection_failed_exception() = default;
    virtual char const* to_string() const noexcept override;
};

class connection_already_disconnected: public vee::exception
{
public:
    using base_t = vee::exception;
    connection_already_disconnected():
        base_t{ "connection already disconnected" }
    {
    }
    virtual ~connection_already_disconnected() = default;
    virtual char const* to_string() const noexcept override;
};

class accept_failed_exception: public vee::exception
{
public:
    using base_t = vee::exception;
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
class  socket_stream;
class  connectable_stream;
using  socket_stream_handle = std::shared_ptr<socket_stream>;
using  connectable_stream_handle = std::shared_ptr<connectable_stream>;
struct async_connect_result;
struct ip_endpoint;

using async_connect_delegate = delegate<void(async_connect_result&)>;
//using async_connect_callback = async_connect_delegate::shared_ptr;
using async_connect_callback = std::function<void(async_connect_result&)>;

struct ip_endpoint //! POD Type
{
    using this_t = ip_endpoint;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<async_connect_result>;
    static const int IP_BUFFER_SIZE = 256;
    char ip[IP_BUFFER_SIZE] { "null" };
    port_t port { 0 };
    ip_endpoint() = default;
    ~ip_endpoint() = default;
    ip_endpoint(const ip_endpoint& other);
    ip_endpoint& operator=(const ip_endpoint& other);
    inline void ip_endpoint::set_value(const char* __ip, port_t __port);
    inline void ip_endpoint::clear();
};

struct async_connect_result : virtual public async_result
{
    using this_t = async_connect_result;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<async_connect_result>;
    connectable_stream_handle stream;
    async_connect_callback callback;
    ip_endpoint endpoint;
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
    virtual ~socket_stream() noexcept = default;  
    virtual ip_endpoint get_endpoint() noexcept = 0;
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
    virtual void connect(const ip_endpoint& endpoint) = 0;
    virtual void disconnect() noexcept = 0;
    virtual void async_connect(const char* ip, port_t port, async_connect_callback callback) noexcept = 0;
    virtual void async_connect(const char* ip, port_t port, async_connect_delegate::shared_ptr callback) noexcept = 0;
    virtual void async_connect(const ip_endpoint& endpoint, async_connect_callback callback) noexcept = 0;
    virtual void async_connect(const ip_endpoint& endpoint, async_connect_delegate::shared_ptr callback) noexcept = 0;
    virtual bool is_open() noexcept = 0;
};

class datagram_stream abstract : virtual public socket_stream
{
public:
    using this_t = datagram_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~datagram_stream() noexcept = default;
    virtual void set_endpoint(const char* ip, port_t port) = 0;
    virtual void set_endpoint(const ip_endpoint& endpoint) = 0;

    // Synchronous I/O member functions 
    virtual size_t read_from(io::buffer buffer, size_t maximum_read_bytes, ip_endpoint* endpoint_out) = 0;
    virtual size_t write_to(io::buffer buffer, const size_t bytes_requested, ip_endpoint& endpoint) = 0;
    // Asynchronous I/O member functions of std::function type callback
    virtual void async_read_from(io::buffer buffer, size_t bytes_requested, async_io_callback callback, ip_endpoint* endpoint_out) noexcept = 0;
    virtual void async_write_to(io::buffer buffer, size_t bytes_requested, async_io_callback callback, ip_endpoint& endpoint) noexcept = 0;
    // Asynchronous I/O member functions of delegate
    virtual void async_read_from(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback, ip_endpoint* endpoint_out) noexcept = 0;
    virtual void async_write_to(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback, ip_endpoint& endpoint) noexcept = 0;

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

namespace rfc6455 {

connectable_stream_handle create_session(io_service& iosvc) noexcept;
tcp::server::shared_ptr create_server(io_service& iosvc, port_t port) noexcept;

} // !namespace rfc6455

} // !namespace tcp

namespace udp {

socket_stream_handle create_stream(io_service& iosvc) noexcept;

} // !namespace udp

} // !namespace ip

} // !namespace net

} // !namespace vee

#endif // !_VEE_NET_H_