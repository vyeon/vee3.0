#ifndef _VEE_COMM_SHARED_MEMORY_H_
#define _VEE_COMM_SHARED_MEMORY_H_

#include <memory>

namespace vee {

namespace comm {

namespace interprocess {

class shared_memory
{
public:
    using this_t = shared_memory;
    using ref_t = this_t&;
    using rref_t = this_t&&;
    using shared_ptr = std::shared_ptr<this_t>;
    using unique_ptr = std::unique_ptr<this_t>;

    enum class create_option
    {
        create_only = 0,
        open_or_create
    };

    enum class authority
    {
        // TODO
    };
    
    virtual ~shared_memory() = default;
    virtual void*        address() const noexcept = 0;
    virtual size_t       size() const noexcept = 0;
    virtual const char*  key() const noexcept = 0;
};

shared_memory::shared_ptr create_shared_memory(const char* key, size_t size, shared_memory::create_option option);

} // !namespace interprocess

} // !namespace comm

} // !namespace vee

#endif // !_VEE_COMM_SHARED_MEMORY_H_