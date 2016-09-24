#ifndef _VEE_IOPORT_IOPORT_SERVICE_H_
#define _VEE_IOPORT_IOPORT_SERVICE_H_

#include <vee/core/noncopyable.h>
#include <memory>

namespace vee {

namespace io {

// Forward declaration
class io_service_kernel;

class io_service: noncopyable
{
public:
    io_service();
    ~io_service();
    std::size_t run() const;
    std::size_t run_one() const;
    void stop() const;
    bool stopped() const;
    std::unique_ptr<io_service_kernel> kernel;
};

} // !namespace io

} // !namespace vee

#endif // !_VEE_IO_SERVICE_H_