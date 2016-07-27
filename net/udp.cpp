#include <vee/io/detail/io_service_kernel.h>
#include <vee/net/udp.h>

namespace vee {

namespace net {

namespace ip {

namespace udp {

namespace /* unnamed */ {
inline ::boost::asio::ip::address string_to_ipaddr(const char* str)
{
    return ::boost::asio::ip::address::from_string(str);
}
} // unnamed namespace

udp_stream::~udp_stream() noexcept
{

}

udp_stream::udp_stream(udp_stream&& other):
    iosvc_ptr{ other.iosvc_ptr },
    socket { std::move(other.socket) }
{
}

udp_stream::udp_stream(io_service& iosvc):
    iosvc_ptr{ &iosvc },
    socket{ iosvc.kernel->to_boost() }
{
}

udp_stream::udp_stream(io_service& iosvc, udp_socket&& __socket):
    iosvc_ptr{ &iosvc },
    socket{ std::move(__socket) }
{
    boost::system::error_code ec;
    auto __remote_endpoint = socket.remote_endpoint(ec);
    if (ec)
    {
        // An error occurred.
    }
    else
    {
        this->remote_endpoint.set_value(__remote_endpoint.address().to_string().c_str(), __remote_endpoint.port());
    }
}

udp_stream::ref_t udp_stream::operator=(udp_stream&& other) noexcept
{
    this->iosvc_ptr = other.iosvc_ptr;
    this->socket = std::move(other.socket);
    this->on_destroy = std::move(other.on_destroy);
    return *this;
}

void udp_stream::swap(ref_t other)
{
    std::swap(iosvc_ptr, other.iosvc_ptr);
    std::swap(socket, other.socket);
    std::swap(on_destroy, other.on_destroy);
}

socketfd_t udp_stream::native() noexcept
{
    return socket.native();
}

void udp_stream::map_remote_endpoint(const char* ip, port_t port) noexcept
{
    remote_endpoint.set_value(ip, port);
}

void udp_stream::map_remote_endpoint(const ip_endpoint& __endpoint) noexcept
{
    remote_endpoint = __endpoint;
}

size_t udp_stream::write_some(const io::buffer& buffer, const size_t bytes_requested)
{
    return 0;
}

size_t udp_stream::read_explicit(io::buffer buffer, const size_t bytes_requested)
{
    return 0;
}

size_t udp_stream::read_some(io::buffer buffer, size_t maximum_read_bytes)
{
    return 0;
}

size_t udp_stream::read_from(io::buffer buffer, size_t maximum_read_bytes, ip_endpoint* endpoint_out)
{
    return 0;
}

size_t udp_stream::write_to(io::buffer buffer, const size_t bytes_requested, ip_endpoint& endpoint)
{
    return 0;
}

void udp_stream::async_read_from(io::buffer buffer, size_t bytes_requested, async_io_callback callback, ip_endpoint* endpoint_out) noexcept
{

}

void udp_stream::async_write_to(io::buffer buffer, size_t bytes_requested, async_io_callback callback, ip_endpoint& endpoint) noexcept
{
}

void udp_stream::async_read_from(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback, ip_endpoint* endpoint_out) noexcept
{
}

void udp_stream::async_write_to(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback, ip_endpoint& endpoint) noexcept
{
}

void udp_stream::async_read_some(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept
{
}

void udp_stream::async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_callback callback) noexcept
{
}

void udp_stream::async_write_some(const io::buffer& buffer, size_t bytes_requested, async_io_callback callback) noexcept
{
}

void udp_stream::async_read_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept
{
}

void udp_stream::async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept
{
}

void udp_stream::async_write_some(const io::buffer& buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept
{
}

ip_endpoint udp_stream::get_remote_endpoint() noexcept
{
    return ip_endpoint{ remote_endpoint };
}

io_service& udp_stream::get_io_service() noexcept
{
    return *iosvc_ptr;
}

} // !namespace tcp

} // !namespace ip

} // !namespace net

} // !namespace vee