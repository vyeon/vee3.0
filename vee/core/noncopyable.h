#ifndef _VEE_CORE_NONCOPYABLE_H_
#define _VEE_CORE_NONCOPYABLE_H_

namespace vee {

class noncopyable
{
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};
    
} // !namespace vee

#endif // !_VEE_CORE_NONCOPYABLE_H_