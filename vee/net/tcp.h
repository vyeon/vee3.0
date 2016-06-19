#ifndef _VEE_NET_TCP_H_
#define _VEE_NET_TCP_H_

#include <vee/net.h>
#include <boost/asio.hpp>

namespace vee {
    
namespace net {
    
namespace tcp {

class tcp_stream;
class tcp_server;

class tcp_stream: virtual public net_stream, noncopyable
{
/* Public member types */
public:
    using this_t = tcp_stream;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
/* Protected member types */
private:
    using tcp_socket = ::boost::asio::ip::tcp::socket;
    using tcp_endpoint = ::boost::asio::ip::tcp::endpoint;

/* Public member functions */
public:
    virtual ~tcp_stream();
    tcp_stream(tcp_stream&& other);
    explicit tcp_stream(io_service& iosvc);
    //tcp_stream& operator=(tcp_stream&& rhs) __noexcept;
    void swap(tcp_stream& other) __noexcept;
    virtual void connect(const char* ip, port_t port) override;
    virtual void disconnect() override;
    virtual void async_connect(const char* ip, port_t port, async_connect_delegate::shared_ptr callback) __noexcept override;
    virtual socketfd_t native() __noexcept override;
    virtual bool is_open() __noexcept override;
    virtual size_t write_some(io::out_buffer_t buffer, const size_t size);
    virtual size_t read_explicit(io::in_buffer_t buffer, const size_t size);
    virtual size_t read_some(io::in_buffer_t buffer, const size_t size);
    virtual void async_read_some(io::in_buffer_t buffer, size_t capacity, async_read_delegate::shared_ptr callback) __noexcept = 0;
    virtual void async_read_explicit(io::in_buffer_t buffer, size_t bytes_requested, async_read_delegate::shared_ptr callback) __noexcept = 0;
    virtual void async_write_some(io::out_buffer_t buffer, size_t bytes_requested, async_write_delegate::shared_ptr callback) __noexcept = 0;
    virtual io_service& get_io_service() __noexcept override;

/* Private member functions */
private:

/* Protected member variables */
protected:
    io_service* _iosvc_ptr;
    tcp_socket _socket;
/* Disallowed member functions */
private:
    // DISALLOW COPY OPERATIONS
    tcp_stream() = delete;
    tcp_stream(const tcp_stream&) = delete;
    void operator=(const tcp_stream&) = delete;
};

class tcp_server: virtual public server, noncopyable
{
/* Public member types */
public:
    using this_t = tcp_server;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
/* Protected member types */
private:
    using tcp_socket = ::boost::asio::ip::tcp::socket;
    using tcp_endpoint = ::boost::asio::ip::tcp::endpoint;
    tcp_server(port_t port, io_service& iosvc);
    tcp_server(tcp_server&& other);
    virtual ~tcp_server();
    virtual void open() override;
    virtual void close() override;
    virtual ::std::pair<bool, session_t> accept() override;
    virtual void async_accept(async_accept_delegate::shared_ptr callback) override;
    virtual io_service& get_io_service() __noexcept override;

/* Protected member variables */
protected:
    io_service*  _iosvc_ptr;
    tcp_socket   _socket;
    tcp_endpoint _endpoint;
    ::boost::asio::ip::tcp::acceptor _acceptor;

/* Disallowed member functions */
private:
    // DISALLOW COPY OPERATIONS
    tcp_server() = delete;
    tcp_server(const tcp_server&) = delete;
    void operator=(const tcp_server&) = delete;
};

} // !namespace tcp

} // !namespace net

} // !namespace vee

#endif // !_VEE_NET_TCP_H_