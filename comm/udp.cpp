#include <vee/io/detail/io_service_kernel.h>
#include <vee/comm/udp.h>

namespace vee {

namespace comm {

namespace ip {

namespace udp {

namespace /* unnamed */ {
inline boost::asio::ip::address string_to_ipaddr(const char* str)
{
    return boost::asio::ip::address::from_string(str);
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

udp_stream::udp_stream(io::io_service& iosvc):
    iosvc_ptr{ &iosvc },
    socket{ iosvc.kernel->to_boost() }
{
}

udp_stream::udp_stream(io::io_service& iosvc, udp_socket&& __socket):
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
        update_remote_endpoint(__remote_endpoint);
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
    remote_endpoint = udp_endpoint{ string_to_ipaddr(ip), port };
}

void udp_stream::map_remote_endpoint(const ip_endpoint& endpoint) noexcept
{
    remote_endpoint = udp_endpoint{ string_to_ipaddr(endpoint.ip), endpoint.port };
}

size_t udp_stream::write_some(const io::buffer& buffer, const size_t bytes_requested)
{
    return this->write_to(buffer, bytes_requested, remote_endpoint_cache);
}

size_t udp_stream::read_explicit(io::buffer buffer, const size_t bytes_requested)
{
    ip_endpoint temp;
    return this->read_from(buffer, bytes_requested, temp);
}

    boost::system::error_code error;
size_t udp_stream::read_some(io::buffer buffer, size_t maximum_read_bytes)
{
    ip_endpoint temp;
    return this->read_from(buffer, maximum_read_bytes, temp);
}

size_t udp_stream::read_from(io::buffer buffer, size_t maximum_read_bytes, ip_endpoint& endpoint_out)
{
    return socket.receive_from(boost::asio::buffer(buffer.ptr, maximum_read_bytes), remote_endpoint);
}

size_t udp_stream::write_to(io::buffer buffer, const size_t bytes_requested, ip_endpoint& endpoint)
{
    return socket.send_to(boost::asio::buffer(buffer.ptr, bytes_requested), remote_endpoint);
}

void udp_stream::async_read_from(io::buffer buffer, size_t bytes_requested, io::async_io_callback callback, ip_endpoint& endpoint_out) noexcept
{
    udp_endpoint* udp_endpoint_out;
    {
        auto raw = static_cast<udp_endpoint*>(udp_endpoint_pool::malloc());
        udp_endpoint_out = new(raw) udp_endpoint;
    }
    auto on_read = [=, &endpoint_out](const ::boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        io::async_io_result result;
        result.stream_ptr = this;
        result.bytes_transferred = bytes_transferred;
        result.buffer = buffer;
        result.callback = callback;
        endpoint_out.set_value(udp_endpoint_out->address().to_string().c_str(), udp_endpoint_out->port());
        if (::boost::asio::error::eof == error)
        {
            result.is_success = false;
            result.issue = io::io_issue::eof;
        }
        else if (error)
        {
            result.is_success = false;
            result.issue = io::io_issue::unknown;
        }
        else
        {
            result.is_success = true;
        }
        callback(result);
        udp_endpoint_out->~udp_endpoint();
        udp_endpoint_pool::free(udp_endpoint_out);
    };
    socket.async_receive_from(boost::asio::buffer(buffer.ptr, bytes_requested), 
                              *udp_endpoint_out,
                              on_read);
    return;
}

void udp_stream::async_write_to(io::buffer buffer, size_t bytes_requested, io::async_io_callback callback, ip_endpoint& endpoint) noexcept
{
}

void udp_stream::async_read_from(io::buffer buffer, size_t bytes_requested, io::async_io_delegate::shared_ptr callback, ip_endpoint& endpoint_out) noexcept
{
}

void udp_stream::async_write_to(io::buffer buffer, size_t bytes_requested, io::async_io_delegate::shared_ptr callback, ip_endpoint& endpoint) noexcept
{
}

void udp_stream::async_read_some(io::buffer buffer, size_t bytes_requested, io::async_io_callback callback) noexcept
{
}

void udp_stream::async_read_explicit(io::buffer buffer, size_t bytes_requested, io::async_io_callback callback) noexcept
{
}

void udp_stream::async_write_some(const io::buffer& buffer, size_t bytes_requested, io::async_io_callback callback) noexcept
{
}

void udp_stream::async_read_some(io::buffer buffer, size_t bytes_requested, io::async_io_delegate::shared_ptr callback) noexcept
{
}

void udp_stream::async_read_explicit(io::buffer buffer, size_t bytes_requested, io::async_io_delegate::shared_ptr callback) noexcept
{
}

void udp_stream::async_write_some(const io::buffer& buffer, size_t bytes_requested, io::async_io_delegate::shared_ptr callback) noexcept
{
}

ip_endpoint udp_stream::get_remote_endpoint() noexcept
{
    return remote_endpoint_cache;
}

io::io_service& udp_stream::get_io_service() const noexcept
{
    return *iosvc_ptr;
}

} // !namespace tcp

} // !namespace ip

} // !namespace comm

} // !namespace vee