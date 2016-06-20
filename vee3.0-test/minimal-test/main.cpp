#include <vee/net.h>
using namespace vee;

int main()
{
    io_service iosvc;
    auto session = net::tcp::create_session(iosvc);
    session->on_destroy += ::std::make_pair(0, []()
    {
        printf("session object is destroyed\n");
    });
    uint8_t buffer[512] = { 0, };
    try
    {
        session->connect("127.0.0.1", 5000);
        while (true)
        {
            gets_s((char*)buffer, 512);
            session->write_some(io::buffer{buffer, 512}, strlen((char*)buffer) + 1);
        }
    }
    catch (vee::exception& e)
    {
        printf("sys> exception: %s\n", e.to_string());
    }
    return 0;
}