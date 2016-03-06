#ifndef _VEE_NET_TCP_H_
#define _VEE_NET_TCP_H_

#include <vee/net.h>
#include <boost/asio.hpp>

namespace vee {
    
namespace net {
    
namespace tcp {

class tcp_stream;
class tcp_server;

class tcp_stream: public net_stream
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
    using io_service = ::boost::asio::io_service;
    using deadline_timer = ::boost::asio::deadline_timer;

/* Public member functions */
public:
    virtual ~tcp_stream();
    tcp_stream(tcp_stream&& other);
    explicit tcp_stream(tcp_socket&& socket);
    explicit tcp_stream(io_service& iosvc);
    //tcp_stream& operator=(tcp_stream&& rhs) __noexcept;
    void swap(tcp_stream& other) __noexcept;
    virtual void connect(const char* ip, port_t port, const size_t timeout) override;
    virtual void disconnect() override;
    virtual void async_connect(async_connect_delegate::shared_ptr info, const size_t timeout) __noexcept override;
    virtual socketfd_t native() __noexcept override;
    virtual size_t write_some(const uint8_t* buffer, const size_t size, const size_t timeout) override;
    virtual size_t read_some(uint8_t* const buffer, const size_t size, const size_t timeout) override;
    virtual void async_read_some(io::async_input_info::shared_ptr info, async_read_delegate::shared_ptr callback, const size_t timeout) __noexcept override;
    virtual void async_write_some(io::async_output_info::shared_ptr info, async_write_delegate::shared_ptr callback, const size_t timeout) __noexcept override;

/* Private member functions */
private:
    void _on_connect(const ::boost::system::error_code& ec,
                     ::boost::asio::ip::tcp::resolver::iterator endpoint_iter);

/* Protected member variables */
protected:
    io_service* _iosvc;
    tcp_socket _socket;
    deadline_timer _deadline;
    deadline_timer _heartbeat;
/* Disallowed member functions */
private:
    // DISALLOW COPY OPERATIONS
    tcp_stream() = delete;
    tcp_stream(const tcp_stream&) = delete;
    void operator=(const tcp_stream&) = delete;
};

class tcp_server: public server
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
    using io_service = ::boost::asio::io_service;
    tcp_server(port_t port, io_service& iosvc);
    tcp_server(tcp_server&& other);
    virtual ~tcp_server();
    virtual void open() override;
    virtual void close() override;
    virtual ::std::pair<bool, session_t> accept() override;
    virtual void async_accept(async_accept_delegate::shared_ptr callback, const size_t timeout) override;

/* Protected member variables */
protected:
    io_service*  _iosvc;
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