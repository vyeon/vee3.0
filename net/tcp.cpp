#include <vee/net/tcp.h>

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
    _socket { ::std::move(other._socket) },
    _deadline { *_iosvc },
    _heartbeat { *_iosvc }
{
}

tcp_stream::tcp_stream(tcp_socket&& socket):
    _iosvc { &(socket.get_io_service()) },
    _socket { ::std::move(socket) },
    _deadline{ *_iosvc },
    _heartbeat{ *_iosvc }
{
}

tcp_stream::tcp_stream(io_service& iosvc):
    _iosvc { &iosvc },
    _socket { *_iosvc },
    _deadline{ *_iosvc },
    _heartbeat{ *_iosvc }
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

void tcp_stream::connect(const char* ip, port_t port, const size_t timeout)
{
    // Set a deadline for the connect operation.
    _deadline.expires_from_now(::boost::posix_time::milliseconds(timeout));
    
    tcp_endpoint ep{ string_to_ipaddr(ip), port };
    ::std::promise<bool> promise;
    ::std::future<bool> future{ promise.get_future() };
    
    // Start the asynchronous connect operation.
    _socket.async_connect(ep, 
                          ::std::bind(&this_t::_on_connect, 
                                      this,
                                      ::std::placeholders::_1,
                                      ::std::move(promise),
                                      ep));
    
    // Block until the asynchronous operation has completed.
    bool result = future.get();

    if (!result)
    {
        throw exl::connection_failed{};
    }
}

void tcp_stream::disconnect()
{

}

void tcp_stream::async_connect(async_connect_delegate::shared_ptr info, const size_t timeout)
{
}

socketfd_t tcp_stream::native()
{
}

size_t tcp_stream::write_some(const uint8_t* buffer, const size_t size, const size_t timeout)
{
}

size_t tcp_stream::read_some(uint8_t* const buffer, const size_t size, const size_t timeout)
{
}

void tcp_stream::async_read_some(io::async_input_info::shared_ptr info, async_read_delegate::shared_ptr callback, const size_t timeout)
{
}

void tcp_stream::async_write_some(io::async_output_info::shared_ptr info, async_write_delegate::shared_ptr callback, const size_t timeout)
{
}

void tcp_stream::_on_connect(const ::boost::system::error_code& ec, 
                             ::std::promise<bool> promise,
                             ::boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
    
}

} // !namespace tcp

} // !namespace net

} // !namespace vee