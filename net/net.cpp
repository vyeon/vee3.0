#include <vee/net.h>

namespace vee {

namespace net {

namespace ip {

ip_endpoint::ip_endpoint(const ip_endpoint& other):
    port{ other.port }
{
    memmove(ip, other.ip, IP_BUFFER_SIZE);
}

ip_endpoint::ref_t ip_endpoint::operator=(const ip_endpoint& other)
{
    memmove(ip, other.ip, IP_BUFFER_SIZE);
    port = other.port;
    return *this;
}

inline void ip_endpoint::set_value(const char* __ip, port_t __port)
{
    strcpy_s(ip, IP_BUFFER_SIZE, __ip);
    port = __port;
}

inline void ip_endpoint::clear()
{
    strcpy_s(ip, IP_BUFFER_SIZE, "null");
    port = 0;
}

} // !namespace ip

} // !namespace net

} // !namespace vee