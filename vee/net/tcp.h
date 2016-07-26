#ifndef _VEE_NET_TCP_H_
#define _VEE_NET_TCP_H_

#include <vee/net.h>
#ifdef VEE_PLATFORM_WINDOWS
#define _WIN32_WINNT 0x0603
#endif
#include <boost/asio/ip/tcp.hpp>
//#include <boost/asio.hpp>

namespace vee {
    
namespace net {

namespace ip {

namespace tcp {

class tcp_stream;
class tcp_server;

class tcp_stream : public connectable_stream, noncopyable
{
    /* Public member types */
public:
    using this_t = tcp_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    /* Protected member types */
private:
    using tcp_socket = ::boost::asio::ip::tcp::socket;
    using tcp_endpoint = ::boost::asio::ip::tcp::endpoint;

    /* Public member functions */
public:
    virtual ~tcp_stream() noexcept;
    tcp_stream(tcp_stream&& other);
    explicit tcp_stream(io_service& iosvc);
    explicit tcp_stream(io_service& iosvc, tcp_socket&& socket);
    tcp_stream& operator=(tcp_stream&& rhs) noexcept;
    void swap(tcp_stream& other) noexcept;
    virtual void connect(const char* ip, port_t port) override;
    virtual void connect(const ip_endpoint& endpoint) override;
    virtual void disconnect() noexcept override;
    virtual void async_connect(const char* ip, port_t port, async_connect_callback callback) noexcept override;
    virtual void async_connect(const char* ip, port_t port, async_connect_delegate::shared_ptr callback) noexcept override;
    virtual void async_connect(const ip_endpoint& endpoint, async_connect_callback callback) noexcept override;
    virtual void async_connect(const ip_endpoint& endpoint, async_connect_delegate::shared_ptr callback) noexcept override;
    virtual socketfd_t native() noexcept override;
    virtual bool is_open() noexcept override;
    virtual size_t write_some(io::buffer buffer, const size_t bytes_requested) override;
    virtual size_t read_explicit(io::buffer buffer, const size_t bytes_requested) override;
    virtual size_t read_some(io::buffer buffer, size_t maximum_read_bytes) override;
    virtual void async_read_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_write_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_read_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual void async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual void async_write_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual ip_endpoint get_endpoint() noexcept override;
    virtual io_service& get_io_service() noexcept override;

    /* Private member functions */
private:

    /* Protected member variables */
protected:
    io_service* iosvc_ptr;
    ip_endpoint endpoint;
    tcp_socket  socket;
    /* Disallowed member functions */
private:
    // DISALLOW COPY OPERATIONS
    tcp_stream() = delete;
    tcp_stream(const tcp_stream&) = delete;
    void operator=(const tcp_stream&) = delete;
};

class tcp_server : public server, noncopyable
{
    /* Public member types */
public:
    using this_t = tcp_server;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
    /* Protected member types */
public:
    using tcp_socket = ::boost::asio::ip::tcp::socket;
    using tcp_endpoint = ::boost::asio::ip::tcp::endpoint;
    tcp_server(io_service& iosvc, port_t port);
    tcp_server(tcp_server&& other);
    virtual ~tcp_server() noexcept;
    virtual void close() noexcept override;
    virtual connectable_stream_handle accept() override;
    //virtual std::pair<bool, session_handle> accept() override;
    virtual void async_accept(async_accept_callback callback) override;
    virtual void async_accept(async_accept_delegate::shared_ptr callback) override;
    virtual io_service& get_io_service() noexcept override;

    /* Protected member variables */
protected:
    io_service*  iosvc_ptr;
    tcp_socket   socket;
    tcp_endpoint endpoint;
    ::boost::asio::ip::tcp::acceptor acceptor;

    /* Disallowed member functions */
private:
    // DISALLOW COPY OPERATIONS
    tcp_server() = delete;
    tcp_server(const tcp_server&) = delete;
    void operator=(const tcp_server&) = delete;
};

} // !namespace tcp

} // !namespace ip

} // !namespace net

} // !namespace vee

#endif // !_VEE_NET_TCP_H_