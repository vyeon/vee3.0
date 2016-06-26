#include <vee/net.h>
#include <Vee/console.h>
using namespace vee;

int main()
{
    io_service iosvc;
    // Create a tcp-server object [Factory method pattern]
    auto server = net::tcp::create_server(iosvc, static_cast<net::port_t>(5000));
    while (true)
    {
        try
        {
            printf("sys> waiting for client connection\n");
            auto session = server->accept();
            printf("sys> client connected!\n");
            const size_t bufsize = 256;
            const size_t maximun_read_bytes = bufsize;
            uint8_t buffer[bufsize] = { 0, };
            while (true)
            {
                size_t bytes_transferred = session->read_some(io::buffer{buffer, bufsize}, maximun_read_bytes);
                printf("%zu bytes from client ----------------------------------------\n", bytes_transferred);
                print_hexa(buffer, bytes_transferred);
                printf("\n------------------------------------------------------------\n");
                
                bytes_transferred = session->write_some(io::buffer{buffer, bufsize}, bytes_transferred);
                printf("%zu bytes to client (ECHO) -----------------------------------\n", bytes_transferred);
                print_hexa(buffer, bytes_transferred);
                printf("\n------------------------------------------------------------\n");
            }
        }
        catch (vee::exception& e)
        {
            printf("err> %s\n", e.to_string());
        }
        
    }
    return 0;
}