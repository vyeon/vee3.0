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

tcp_stream::~tcp_stream() noexcept
{
    on_destroy.do_call();
}

tcp_stream::tcp_stream(tcp_stream&& other):
    iosvc_ptr { other.iosvc_ptr },
    socket { ::std::move(other.socket) }
{
}


tcp_stream::tcp_stream(io_service& iosvc):
    iosvc_ptr { &iosvc },
    socket { iosvc.kernel->to_boost() }
{
}

tcp_stream::tcp_stream(io_service& iosvc, tcp_socket&& __socket):
    iosvc_ptr { &iosvc },
    socket { ::std::move(__socket)}
{
}

void tcp_stream::swap(tcp_stream & other) noexcept
{
    ::std::swap(iosvc_ptr, other.iosvc_ptr);
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
    ::std::string _ip_str{ ip };
    auto on_connect = [callback, port, ip_str = ::std::move(_ip_str)](const ::boost::system::error_code& error)
    {
        async_connection_result result;
        result.ip = ::std::move(ip_str);
        result.port = port;
        result.callback = ::std::move(callback);
        char buffer[256] = { 0, };
        _itoa_s(port, buffer, 10);
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
         callback->do_call(result);
    };
    tcp_endpoint ep{string_to_ipaddr(ip), port};
    socket.async_connect(ep, on_connect);
}

socketfd_t tcp_stream::native() __noexcept
{
    return socket.native();
}

bool tcp_stream::is_open() __noexcept
{
    return socket.is_open();
}

size_t tcp_stream::write_some(io::buffer buffer, const size_t bytes_requested)
{
    ::boost::system::error_code error;
    size_t bytes_transferred = 0;
    while (bytes_transferred != bytes_requested)
    {
        bytes_transferred += static_cast<size_t>(socket.write_some(::boost::asio::buffer(buffer.ptr + bytes_transferred, bytes_requested - bytes_transferred), error));
        if (boost::asio::error::eof == error)
        {
            throw io::invalid_stream_exception{};
        }
        else if (boost::asio::error::connection_reset == error)
        {
            throw io::stream_reset_exception{};
        }
        else if (error)
        {
            throw io::unknown_io_exception{};
        }
    }
    return bytes_transferred;
}

size_t tcp_stream::read_explicit(io::buffer buffer, const size_t bytes_requested)
{
    size_t bytes_transferred = 0;
    while (bytes_transferred == bytes_requested)
    {
        bytes_transferred += read_some(buffer, bytes_requested);
        buffer.ptr += bytes_transferred;
        buffer.capacity -= bytes_transferred;
    }
    return bytes_transferred;
}

size_t tcp_stream::read_some(io::buffer buffer, size_t maximum_read_bytes)
{
    ::boost::system::error_code error;
    memset(buffer.ptr, 0, buffer.capacity);
    size_t bytes_transferred = static_cast<size_t>(socket.read_some(::boost::asio::buffer(buffer.ptr, maximum_read_bytes), error));
    if (boost::asio::error::eof == error)
    {
        throw io::invalid_stream_exception{};
    }
    else if (boost::asio::error::connection_reset == error)
    {
        throw io::stream_reset_exception{};
    }
    else if (error)
    {
        throw io::unknown_io_exception{};
    }
    return bytes_transferred;
}

void tcp_stream::async_read_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept
{
    auto on_read = [buffer, callback](const ::boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        async_io_result result;
        result.bytes_transferred = bytes_transferred;
        result.buffer = buffer;
        result.callback = callback;
        if (::boost::asio::error::eof == error)
        {
            result.is_success = false;
            result.issue = io_issue::eof;
        }
        else if (::boost::asio::error::connection_reset == error)
        {
            result.is_success = false;
            result.issue = io_issue::connection_reset;
        }
        else if (error)
        {
            result.is_success = false;
            result.issue = io_issue::unknown;
        }
        else
        {
            result.is_success = true;
            // result.issue = io_issue::null; // default
        }
        callback->do_call(result);
    };
    socket.async_read_some(::boost::asio::buffer(buffer.ptr, bytes_requested), on_read);
    return;
}

void tcp_stream::async_read_explicit(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept
{
    auto iteration = [this, buffer, bytes_requested, callback, total_transferred = static_cast<size_t>(0)](async_io_result& result) mutable -> void
    {
        total_transferred += result.bytes_transferred;
        if (!result.is_success || total_transferred == bytes_requested)
        {
            result.buffer = buffer;
            result.bytes_transferred = total_transferred;
            result.callback = callback;
            callback->do_call(result);
            return;
        }
        result.buffer.ptr += result.bytes_transferred;
        result.buffer.capacity -= result.bytes_transferred;
        tcp_stream::async_read_some(result.buffer, bytes_requested - total_transferred, result.callback);
    };
    tcp_stream::async_read_some(buffer, bytes_requested, ::std::make_shared<async_io_delegate>(0, iteration));
}

void tcp_stream::async_write_some(io::buffer buffer, size_t bytes_requested, async_io_delegate::shared_ptr callback) noexcept
{
    auto on_write = [buffer, callback](const ::boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        async_io_result result;
        result.bytes_transferred = bytes_transferred;
        result.buffer = buffer;
        result.callback = callback;
        if (::boost::asio::error::eof == error)
        {
            result.is_success = false;
            result.issue = io_issue::eof;
        }
        else if (::boost::asio::error::connection_reset == error)
        {
            result.is_success = false;
            result.issue = io_issue::connection_reset;
        }
        else if (error)
        {
            result.is_success = false;
            result.issue = io_issue::unknown;
        }
        else
        {
            result.is_success = true;
            // result.issue = io_issue::null; // default
        }
        callback->do_call(result);
    };
    socket.async_write_some(::boost::asio::buffer(buffer.ptr, bytes_requested), on_write);
    return;
}

io_service& tcp_stream::get_io_service() __noexcept
{
    return *iosvc_ptr;
}

tcp_server::tcp_server(io_service& iosvc, port_t port):
    iosvc_ptr{ &iosvc },
    socket{ iosvc.kernel->to_boost() },
    endpoint{ boost::asio::ip::tcp::v4(), port },
    acceptor { iosvc.kernel->to_boost(), endpoint }
{
}

tcp_server::tcp_server(tcp_server&& other):
    iosvc_ptr { other.iosvc_ptr },
    socket { ::std::move(other.socket)},
    endpoint { ::std::move(other.endpoint)},
    acceptor { ::std::move(other.acceptor)}
{
}

tcp_server::~tcp_server() noexcept
{
    tcp_server::close();
}

void tcp_server::close() noexcept
{
    if (acceptor.is_open())
    {
        acceptor.close();
    }
    if (socket.is_open())
    {
        socket.close();
    }
}

session_t tcp_server::accept()
{
    ::boost::asio::ip::tcp::socket client(iosvc_ptr->kernel->to_boost());
    try
    {
        acceptor.accept(client);
    }
    catch (::std::exception&)
    {
        throw accept_failed_exception();
    }
    session_t session = ::std::make_shared<tcp_stream>(*iosvc_ptr, std::move(client));
    return session;
}

void tcp_server::async_accept(async_accept_delegate::shared_ptr callback)
{
    ::std::shared_ptr<::boost::asio::ip::tcp::socket> clntsock_ptr = std::make_shared<::boost::asio::ip::tcp::socket>(iosvc_ptr->kernel->to_boost());
    auto on_accept = [this, clntsock_ptr, callback](const ::boost::system::error_code& error)
    {
        async_accept_result result;
        result.callback = ::std::move(callback);
        if (error)
        {
            result.is_success = false;
            result.message += error.message();
        }
        else
        {
            result.is_success = true;
            result.message += error.message();
            //TODO: I think this is dangerous code. I will resolve this.
            result.session = ::std::make_shared<tcp_stream>(*iosvc_ptr, ::std::move(*clntsock_ptr));
        }
        callback->do_call(result);
    };
    acceptor.async_accept(*clntsock_ptr, on_accept);
}

io_service& tcp_server::get_io_service() noexcept
{
    return *iosvc_ptr;
}

session_t create_session(io_service& iosvc) noexcept
{
    session_t session = ::std::make_shared<tcp_stream>(iosvc);
    return session;
}

server::shared_ptr create_server(io_service& iosvc, port_t port) noexcept
{
    server::shared_ptr server = ::std::make_shared<tcp_server>(iosvc, port);
    return server;
}

} // !namespace tcp

} // !namespace net

} // !namespace vee