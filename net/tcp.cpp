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
    _socket { ::std::move(other._socket) }
{
}

tcp_stream::tcp_stream(io_service& iosvc):
    _iosvc_ptr { &iosvc },
    _socket { iosvc.kernel->to_boost() }
{
}

//tcp_stream& tcp_stream::operator=(tcp_stream&& rhs) __noexcept
//{
//    disconnect();
//    _socket = ::std::move(rhs._socket);
//    iosvc = rhs.iosvc;
//    return *this;
//}

void tcp_stream::swap(tcp_stream& other) __noexcept
{
    ::std::swap(_iosvc_ptr, other._iosvc_ptr);
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

io_service& tcp_stream::get_io_service() __noexcept
{
    return *_iosvc_ptr;
}

} // !namespace tcp

} // !namespace net

} // !namespace vee