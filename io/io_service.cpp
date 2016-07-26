#include <vee/io/io_service.h>
#include <vee/io/detail/io_service_kernel.h>

namespace vee {

io_service::io_service():
    kernel { std::make_unique<io_service_kernel>() }
{
}

io_service::~io_service()
{
}

std::size_t io_service::run()
{
    return kernel->run();
}

std::size_t io_service::run_one()
{
    return kernel->run_one();
}

void io_service::stop()
{
    return kernel->stop();
}

bool io_service::stopped() const
{
    return kernel->stopped();
}

} // !namespace vee