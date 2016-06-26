#include <vee/net.h>
#include <vee/console.h>
using namespace vee;

int main()
{
    io_service iosvc; 
    while (true)
    {
        try
        {
            // Create a tcp-client object [Factory method pattern]
            auto client = net::tcp::create_session(iosvc);
            printf("---------------------------\n");
            printf("sys> test client initalized\n");

            char ip_buffer[64] = { 0, };
            printf("in > connect to:\n\tip> ");
            gets_s(ip_buffer);

            if (strlen(ip_buffer) <= 0)
                throw ::vee::exception{ "invalid IP address" };
            
            char port_buffer[16] = { 0, };
            printf("\tport> ");
            gets_s(port_buffer);
            
            printf("sys> trying to connect... (%s:%hu)\n", ip_buffer, atoi(port_buffer));
            client->connect(ip_buffer, atoi(port_buffer));

            while (true)
            {
                const size_t MSG_BUFFER_SIZE = 512;
                uint8_t msg_buffer[MSG_BUFFER_SIZE] = { 0, };
                printf("sys> connection success!\nin >\tmsg> ");
                gets_s(reinterpret_cast<char*>(msg_buffer), MSG_BUFFER_SIZE);
                size_t bytes_transferred = 
                    client->write_some(io::buffer{ msg_buffer, MSG_BUFFER_SIZE },
                                       strlen(reinterpret_cast<char*>(msg_buffer)));
                printf("%zu bytes to server ------------------------------------------\n", bytes_transferred);
                print_hexa(msg_buffer, bytes_transferred);
                printf("\n------------------------------------------------------------\n");

                printf("sys> waiting for response from server...\n");
                memset(msg_buffer, 0, MSG_BUFFER_SIZE);
                bytes_transferred =
                    client->read_some(io::buffer{ msg_buffer, MSG_BUFFER_SIZE }, MSG_BUFFER_SIZE);
                printf("%zu bytes from server ----------------------------------------\n", bytes_transferred);
                print_hexa(msg_buffer, bytes_transferred);
                printf("\n------------------------------------------------------------\n");
            }
            
        }
        catch (::vee::exception& e)
        {
            printf("err> %s\n", e.to_string());
        }
    }
    return 0;
}