#ifndef _VEE_IO_DETAIL_IO_SERVICE_KERNEL_H_
#define _VEE_IO_DETAIL_IO_SERVICE_KERNEL_H_

#include <boost/asio/io_service.hpp>

namespace vee {
    
class io_service_kernel: public ::boost::asio::io_service
{
public:
    inline ::boost::asio::io_service& to_boost()
    {
        return reinterpret_cast<::boost::asio::io_service&>(*this);
    }
};

} // !namespace vee

#endif // !_VEE_IO_DETAIL_IO_SERVICE_KERNEL_H_