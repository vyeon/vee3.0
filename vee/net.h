#ifndef _VEE_NET_H_
#define _VEE_NET_H_

#include <vee/stream.h>
#include <vee/platform.h>
#include <string>

namespace vee {
   
namespace net {

using port_t = unsigned short;
#if VEE_PLATFORM_X32
using socketfd_t = uint32_t;
#elif VEE_PLATFORM_X64
using socketfd_t = uint64_t;
#endif // VEE_PLATFORM

struct async_connect_info
{
    using shared_ptr = ::std::shared_ptr<async_connect_info>;
    bool is_success;
};

class net_stream abstract: public io_stream
{
public:
    using this_t = net_stream;
    using ref_t  = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = ::std::shared_ptr<this_t>;
    using unique_ptr = ::std::unique_ptr<this_t>;
    using async_connect_delegate = delegate<void(async_connect_info::shared_ptr)>;
    virtual ~net_stream() = default;
    virtual void connect(const char* ip, port_t port) __PURE;
    virtual void disconnect() __PURE;
    virtual void async_connect(async_connect_delegate::shared_ptr info);
};
    
} // !namespace net

} // !namespace vee

#endif _VEE_NET_H_