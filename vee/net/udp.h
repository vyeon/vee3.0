#ifndef _VEE_NET_UDP_H_
#define _VEE_NET_UDP_H_

#include <vee/net.h>
#ifdef VEE_PLATFORM_WINDOWS
#define _WIN32_WINNT 0x0603
#endif // !VEE_PLATFORM WINDOWS
#include <boost/asio/ip/udp.hpp>

namespace vee {

namespace net {

namespace ip {

namespace udp {

class udp_stream;

class udp_stream: public datagram_stream, noncopyable
{
public:
    using this_t = udp_stream;
    using ref_t  = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;
private:
    using udp_socket = boost::asio::ip::udp::socket;
    using udp_endpoint = boost::asio::ip::udp::endpoint;
public:
    virtual ~udp_stream() noexcept;
    udp_stream(udp_stream&& other);
    explicit udp_stream(io_service& iosvc);
    explicit udp_stream(io_service& iosvc, udp_socket&& socket);
    ref_t operator=(udp_stream&& other) noexcept;
    void swap(ref_t other);
    virtual socketfd_t native() noexcept override;
    virtual void set_endpoint(const char* ip, port_t port) noexcept override;
    virtual void set_endpoint(const ip_endpoint& endpoint) noexcept override;
    // Synchronous I/O memver functions
    virtual size_t write_some(io::buffer buffer, const size_t bytes_requested) override;
    virtual size_t read_explicit(io::buffer buffer, const size_t bytes_requested) override;
    virtual size_t read_some(io::buffer buffer, size_t maximum_read_bytes) override; 
    virtual size_t write_some_to(ip_endpoint& endpoint, io::buffer buffer, const size_t bytes_requested) override;
    virtual size_t read_explicit_to(ip_endpoint& endpoint, io::buffer buffer, const size_t bytes_requested) override;
    virtual size_t read_some_to(ip_endpoint& endpoint, io::buffer buffer, size_t maximum_read_bytes) override;
    // Asynchronous I/O member functions of std::function type callback
    virtual void async_read_some_to(ip_endpoint& endpoint, io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_read_explicit_to(ip_endpoint& endpoint, io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_write_some_to(ip_endpoint& endpoint, io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_read_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    virtual void async_write_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept override;
    // Asynchronous I/O member functions of delegate
    virtual void async_read_some_to(ip_endpoint& endpoint, io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual void async_read_explicit_to(ip_endpoint& endpoint, io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual void async_write_some_to(ip_endpoint& endpoint, io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual void async_read_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual void async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    virtual void async_write_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept override;
    
    virtual ip_endpoint get_endpoint() noexcept override;
    virtual io_service& get_io_service() noexcept override;
/* Protected member variables */
protected:
    io_service* iosvc_ptr;
    ip_endpoint endpoint;
    udp_socket socket;
/* Disabled member functions */
private:
    udp_stream() = delete;
    udp_stream(const udp_stream&) = delete;
    void operator=(const udp_stream&) = delete;
};

} // !namespace tcp

} // !namespace ip

} // !namespace net

} // !namespace vee

#endif // !_VEE_NET_UDP_H_