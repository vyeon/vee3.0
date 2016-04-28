#include <vee/net/tcp.h>
#include <vee/exl.h>

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
    _iosvc { other._iosvc },
    _socket { ::std::move(other._socket) }
{
}

tcp_stream::tcp_stream(tcp_socket&& socket):
    _iosvc { &(socket.get_io_service()) },
    _socket { ::std::move(socket) }
{
}

tcp_stream::tcp_stream(io_service& iosvc):
    _iosvc { &iosvc },
    _socket { *_iosvc }
{
}

//tcp_stream& tcp_stream::operator=(tcp_stream&& rhs) __noexcept
//{
//    disconnect();
//    _socket = ::std::move(rhs._socket);
//    _iosvc = rhs._iosvc;
//    return *this;
//}

void tcp_stream::swap(tcp_stream& other) __noexcept
{
    ::std::swap(_iosvc, other._iosvc);
    ::std::swap(_socket, other._socket);
}

void tcp_stream::connect(const char* ip, port_t port)
{
    
}

void tcp_stream::disconnect()
{
    if (!_socket.is_open())
        throw exl::net::connection_already_disconnected{};
    _socket.close();
}

void tcp_stream::async_connect(async_connect_info::shared_ptr info, async_connect_delegate::shared_ptr callback) __noexcept
{

}

socketfd_t tcp_stream::native() __noexcept
{
    return _socket.native();
}

bool tcp_stream::is_open() __noexcept
{
    return _socket.is_open();
}

size_t tcp_stream::write_some(const uint8_t* buffer, const size_t size)
{
    return 0;
}

size_t tcp_stream::read_some(uint8_t* const buffer, const size_t size)
{
    return 0;
}

void tcp_stream::async_read_some(io::async_input_info::shared_ptr info, async_read_delegate::shared_ptr callback) __noexcept
{

}

void tcp_stream::async_write_some(io::async_output_info::shared_ptr info, async_write_delegate::shared_ptr callback) __noexcept
{

}

} // !namespace tcp

} // !namespace net

} // !namespace vee