#include <vee/net/tcp.h>
#include <vee/io/detail/io_service_kernel.h>

namespace vee {
    
namespace net {
    
namespace tcp {

namespace /* unnamed */ {
inline ::boost::asio::ip::address string_to_ipaddr(const char* str)
{
    return ::boost::asio::ip::address::from_string(str);
}
} // unnamed namespace

tcp_stream::~tcp_stream()
{
}

tcp_stream::tcp_stream(tcp_stream&& other):
    _iosvc_ptr { other._iosvc_ptr },
    socket { ::std::move(other.socket) }
{
}


tcp_stream::tcp_stream(io_service& iosvc):
    _iosvc_ptr { &iosvc },
    socket { iosvc.kernel->to_boost() }
{
}

void tcp_stream::swap(tcp_stream& other) __noexcept
{
    ::std::swap(_iosvc_ptr, other._iosvc_ptr);
    ::std::swap(socket, other.socket);
}

void tcp_stream::connect(const char* ip, port_t port)
{
    ::boost::system::error_code err;
    tcp_endpoint ep{ string_to_ipaddr(ip), port };
    socket.connect(ep, err);
    if (err)
    {
        throw connection_failed_exception{};
    }
}

void tcp_stream::disconnect()
{
    if (!socket.is_open())
        throw connection_already_disconnected{};
    socket.close();
}

void tcp_stream::async_connect(const char* ip, port_t port, async_connect_delegate::shared_ptr callback) __noexcept
{
    auto on_connect = [callback, port](::std::string&& _ip, const ::boost::system::error_code& error)
    {
        async_connection_result result;
        result.ip = ::std::move(_ip);
        result.port = port;
        result.callback = ::std::move(callback);
        char buffer[256] = { 0, };
        itoa(port, buffer, 10);
         if (error)
         {
             result.is_success = false;
             result.message = result.ip + ":" + buffer + " connection failed, detail:" + error.message();
         }
         else
         {
             result.is_success = true;
             result.message = result.ip + ":" + buffer + " connection established";
         }
         callback->do_call(::std::move(result));
    };
    tcp_endpoint ep{string_to_ipaddr(ip), port};
    socket.async_connect(ep, ::std::bind(on_connect, ::std::string(ip), ::std::placeholders::_1));
}

socketfd_t tcp_stream::native() __noexcept
{
    return socket.native();
}

bool tcp_stream::is_open() __noexcept
{
    return socket.is_open();
}

size_t tcp_stream::write_some(io::out_buffer_t buffer, const size_t size)
{
    ::boost::system::error_code error;
    size_t bytes_transferred = static_cast<size_t>(socket.write_some(::boost::asio::buffer(buffer, size), error));
    if (boost::asio::error::eof == error)
    {
        throw io::invalid_stream_exception{};
    }
    else if (boost::asio::error::connection_reset == error)
    {
        throw io::stream_corrupted_exception{};
    }
    else if (error)
    {
        throw io::unknown_io_exception{};
    }
    return bytes_transferred;
}

size_t tcp_stream::read_explicit(io::in_buffer_t buffer, const size_t size)
{

}

size_t tcp_stream::read_some(io::in_buffer_t buffer, const size_t size)
{
}

io_service& tcp_stream::get_io_service() __noexcept
{
    return *_iosvc_ptr;
}

tcp_server::tcp_server(port_t port, io_service& iosvc):
    _iosvc_ptr { &iosvc },
    _socket{ iosvc.kernel->to_boost() },
    _acceptor { iosvc.kernel->to_boost() }
{
}

tcp_server::tcp_server(tcp_server&& other):
    _iosvc_ptr { other._iosvc_ptr },
    _socket { ::std::move(other._socket)},
    _acceptor { ::std::move(other._acceptor)}
{
}

tcp_server::~tcp_server()
{
}

void tcp_server::open()
{
}

void tcp_server::close()
{
}

::std::pair<bool, session_t> tcp_server::accept()
{
}

void tcp_server::async_accept(async_accept_delegate::shared_ptr callback)
{
}

io_service& tcp_server::get_io_service()
{
    return *_iosvc_ptr;
}

} // !namespace tcp

} // !namespace net

} // !namespace vee