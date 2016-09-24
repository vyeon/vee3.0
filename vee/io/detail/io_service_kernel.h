#ifndef _VEE_IOPORT_DETAIL_IOPORT_SERVICE_KERNEL_H_
#define _VEE_IOPORT_DETAIL_IOPORT_SERVICE_KERNEL_H_

#include <vee/platform.h>
#ifdef VEE_PLATFORM_WINDOWS
#define _WIN32_WINNT 0x0603
#endif
#include <boost/asio/io_service.hpp>

namespace vee {
   
namespace io {

class io_service_kernel: public ::boost::asio::io_service
{
public:
    inline ::boost::asio::io_service& to_boost()
    {
        return reinterpret_cast<::boost::asio::io_service&>(*this);
    }
};

} // !namespace io

} // !namespace vee

#endif // !_VEE_IOPORT_DETAIL_IOPORT_SERVICE_KERNEL_H_