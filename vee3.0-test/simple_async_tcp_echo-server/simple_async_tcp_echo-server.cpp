#include <vee/net.h>
#include <vee/console.h>
using namespace vee;

void on_accept(net::ip::tcp::async_accept_result& result);
void on_send(net::ip::connectable_stream_handle s, async_io_result& result);
void on_recv(net::ip::connectable_stream_handle s, async_io_result& result);
void buffer_deleter(uint8_t* dst);

const size_t IO_BUFFER_SIZE = 512;

int main()
{
    io_service iosvc;
    //Create a tcp-server object [Factory method pattern]
    auto server = net::ip::tcp::create_server(iosvc, static_cast<net::ip::port_t>(5000));
    server->async_accept(on_accept);
    iosvc.run();
    return 0;
}

void on_accept(net::ip::tcp::async_accept_result& result)
{
    if (result.is_success)
    {
        uint8_t* io_buffer = new uint8_t[IO_BUFFER_SIZE];
        result.session->on_destroy += std::make_pair(NULL, std::bind(buffer_deleter, io_buffer));
        result.session->async_read_some(
            io::buffer{ io_buffer, IO_BUFFER_SIZE }, 
            IO_BUFFER_SIZE, 
            std::bind(on_recv, result.session, std::placeholders::_1));
        result.server_ptr->async_accept(result.callback);
    }
    else
    {
        printf("err> server acceptor failed!\n");
    }
}

void on_send(net::ip::connectable_stream_handle stream, async_io_result& result)
{
    if (!result.is_success)
    {
        printf("Failed to send data to client. This stream will be destroyed!\n");
        return;
    }
    
    printf("%zu bytes to client (ECHO) -----------------------------------\n", result.bytes_transferred);
    print_hexa(result.buffer.ptr, result.bytes_transferred);
    printf("\n------------------------------------------------------------\n");
    stream->async_read_some(
        result.buffer,
        IO_BUFFER_SIZE,
        std::bind(on_recv, stream, std::placeholders::_1));
}

void on_recv(net::ip::connectable_stream_handle session, async_io_result& result)
{
    if (!result.is_success)
    {
        printf("Failed to recv data to client. This session will be destroyed!\n");
        return;
    }

    printf("%zu bytes from client ----------------------------------------\n", result.bytes_transferred);
    print_hexa(result.buffer.ptr, result.bytes_transferred);
    printf("\n------------------------------------------------------------\n");
    session->async_write_some(
        result.buffer, 
        result.bytes_transferred, 
        std::bind(on_send, session, std::placeholders::_1)
    );
}

void buffer_deleter(uint8_t* dst)
{
    printf("");
    delete[] dst;
}
