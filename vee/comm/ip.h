#ifndef _VEE_COMM_IP_H_
#define _VEE_COMM_IP_H_

#include <vee/io.h>
#include <vee/platform.h>
// ReSharper disable CppUnusedIncludeDirective
#include <vee/comm.h>
// ReSharper restore CppUnusedIncludeDirective

namespace vee {

namespace comm {

namespace ip {

using port_t = unsigned short;
#if VEE_PLATFORM_X32
using socketfd_t = uint32_t;
#elif VEE_PLATFORM_X64
using socketfd_t = uint64_t;
#endif // VEE_PLATFORM

// Forward declaration
struct async_connect_result;
struct ip_endpoint;

class socket_interface: public io::io_port
{
public:
    using this_t = socket_interface;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual socketfd_t native() noexcept = 0;
    virtual ~socket_interface() noexcept = default;
    virtual ip_endpoint get_remote_endpoint() noexcept = 0;
};

using async_connect_delegate = delegate<void(async_connect_result&)>;
using async_connect_callback = std::function<void(async_connect_result&)>;

class stream_socket: virtual public socket_interface
{
public:
    using this_t = stream_socket;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~stream_socket() noexcept = default;
    virtual void connect(const char* ip, port_t port) = 0;
    virtual void connect(const ip_endpoint& endpoint) = 0;
    virtual void disconnect() noexcept = 0;
    virtual void async_connect(const char* ip, port_t port, async_connect_callback callback) noexcept = 0;
    virtual void async_connect(const char* ip, port_t port, async_connect_delegate::shared_ptr callback) noexcept = 0;
    virtual void async_connect(const ip_endpoint& endpoint, async_connect_callback callback) noexcept = 0;
    virtual void async_connect(const ip_endpoint& endpoint, async_connect_delegate::shared_ptr callback) noexcept = 0;
    virtual bool is_open() noexcept = 0;
};

class datagram_socket: virtual public socket_interface
{
public:
    using this_t = datagram_socket;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~datagram_socket() noexcept = default;
    virtual void map_remote_endpoint(const char* ip, port_t port) = 0;
    virtual void map_remote_endpoint(const ip_endpoint& endpoint) = 0;

    // Synchronous I/O member functions 
    virtual size_t read_from(io::buffer buffer, size_t maximum_read_bytes, ip_endpoint& endpoint_out) = 0;
    virtual size_t write_to(io::buffer buffer, const size_t bytes_requested, ip_endpoint& endpoint) = 0;
    // Asynchronous I/O member functions for std::function type callback
    virtual void async_read_from(io::buffer buffer, size_t bytes_requested, io::async_io_callback callback, ip_endpoint& endpoint_out) noexcept = 0;
    virtual void async_write_to(io::buffer buffer, size_t bytes_requested, io::async_io_callback callback, ip_endpoint& endpoint) noexcept = 0;
    // Asynchronous I/O member functions for vee::delegate type callback
    virtual void async_read_from(io::buffer buffer, size_t bytes_requested, io::async_io_delegate::shared_ptr callback, ip_endpoint& endpoint_out) noexcept = 0;
    virtual void async_write_to(io::buffer buffer, size_t bytes_requested, io::async_io_delegate::shared_ptr callback, ip_endpoint& endpoint) noexcept = 0;
};

struct ip_endpoint //! POD Type
{
    using this_t = ip_endpoint;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    static const int IP_BUFFER_SIZE = 256;
    char ip[IP_BUFFER_SIZE] { "null" };
    port_t port { 0 };
    ip_endpoint() = default;
    ~ip_endpoint() = default;
    ip_endpoint(const char* __ip, port_t __port);
    ip_endpoint(const ip_endpoint& other);
    ip_endpoint& operator=(const ip_endpoint& other);
    void set_value(const char* __ip, port_t __port);
    void clear();
};

struct async_connect_result : public io::async_result
{
    using this_t = async_connect_result;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    stream_socket::shared_ptr stream;
    async_connect_callback callback;
    ip_endpoint endpoint;
    std::string message;
};

namespace tcp {

struct async_accept_result;

using async_accept_delegate = delegate<void(async_accept_result&)>;
//using async_accept_callback = async_accept_delegate::shared_ptr;
using async_accept_callback = std::function<void(async_accept_result&)>;

class server_t
{
public:
    using this_t = server_t;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    virtual ~server_t() noexcept = default;
    virtual void close() noexcept = 0;
    virtual stream_socket::shared_ptr accept() = 0;
    virtual void async_accept(async_accept_callback callback) = 0;
    virtual void async_accept(async_accept_delegate::shared_ptr callback) = 0;
    virtual io::io_service& get_io_service() const noexcept = 0;
};

struct async_accept_result : public io::async_result
{
    using shared_ptr = std::shared_ptr<async_accept_result>;
    server_t* server_ptr{ nullptr };
    stream_socket::shared_ptr session;
    std::string message;
    async_accept_callback callback;
};

stream_socket::shared_ptr create_stream(io::io_service& iosvc) noexcept;
server_t::shared_ptr create_server(io::io_service& iosvc, port_t port) noexcept;

namespace rfc6455 {

//stream_socket::shared_ptr create_stream(io::io_service& iosvc) noexcept;
//tcp::server::shared_ptr create_server(io::io_service& iosvc, port_t port) noexcept;

} // !namespace rfc6455

} // !namespace tcp

namespace udp {

//socket_stream_handle create_stream(io::io_service& iosvc) noexcept;

} // !namespace udp

} // !namespace ip

} // !namespace comm

} // !namespace vee

#endif // !_VEE_COMM_IP_H_