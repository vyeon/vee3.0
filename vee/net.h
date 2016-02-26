#ifndef _VEE_NET_H_
#define _VEE_NET_H_

#include <vee/stream.h>
#include <string>

namespace vee {
    
using port_t = unsigned short;

namespace net {

struct operation_result
{
    bool    is_success = false;
};

struct async_connect_info
{
    using shared_ptr = ::std::shared_ptr<async_connect_info>;
    operation_result result;
};

class net_stream abstract: public vee_stream
{
public:
    using async_connect_delegate = delegate<void(async_connect_info::shared_ptr)>;
};
    
} // !namespace net

} // !namespace vee

#endif _VEE_NET_H_