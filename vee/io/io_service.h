#ifndef _VEE_IO_SERVICE_H_
#define _VEE_IO_SERVICE_H_

#include <vee/core/noncopyable.h>
#include <memory>

namespace vee {

// Forward declaration
class io_service_kernel;

class io_service: noncopyable
{
public:
    io_service();
    ~io_service();
    std::size_t run();
    std::size_t run_one();
    void stop();
    bool stopped() const;
    std::unique_ptr<io_service_kernel> kernel;
};

} // !namespace vee

#endif // !_VEE_IO_SERVICE_H_