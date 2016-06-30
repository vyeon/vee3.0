#include <vee/net.h>
#include <vee/console.h>
using namespace vee;
using namespace net;

void on_accept(tcp::async_accept_result& result);
void on_send(session_t session, async_io_result& result);
void on_recv(session_t session, async_io_result& result);

const size_t IO_BUFFER_SIZE = 512;

int main()
{
    io_service iosvc;
    // Create a tcp-server object [Factory method pattern]
    auto server = tcp::create_server(iosvc, static_cast<port_t>(5000));
    server->async_accept(on_accept);
    return 0;
}

void on_accept(tcp::async_accept_result& result)
{
    if (result.is_success)
    {
        uint8_t* io_buffer = new uint8_t[IO_BUFFER_SIZE];
        result.session->on_destroy += ::std::make_pair( NULL, [io_buffer]()
        {
            delete io_buffer;
        });
        result.session->async_read_some(
            io::buffer{ io_buffer, IO_BUFFER_SIZE }, 
            IO_BUFFER_SIZE, 
            async_callback<async_io_callback>( NULL, ::std::bind(on_recv, result.session, ::std::placeholders::_1) ));
        result.server_ptr->async_accept(result.callback);
    }
    else
    {
        printf("err> server acceptor failed!\n");
    }
}

void on_send(session_t session, async_io_result& result)
{
    printf("%zu bytes to client (ECHO) -----------------------------------\n", result.bytes_transferred);
    print_hexa(result.buffer.ptr, result.bytes_transferred);
    printf("\n------------------------------------------------------------\n");
}

void on_recv(session_t session, async_io_result& result)
{
    
}
