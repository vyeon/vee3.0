#ifndef _VEE_PLATFORM_H_
#define _VEE_PLATFORM_H_

namespace vee {

// Check windows
#if _WIN32 || _WIN64
#define VEE_PLATFORM_WINDOWS 1
#if _WIN64
#define VEE_PLATFORM_X32 0
#define VEE_PLATFORM_X64 1
#else
#define VEE_PLATFORM_X32 1
#define VEE_PLATFORM_X64 0
#endif
#endif

// Check GCC
#if __GNUC__
#define VEE_PLATFORM_WINDOWS 0
#if __x86_64__ || __ppc64__
#define VEE_PLATFORM_X32 0
#define VEE_PLATFORM_X64 1
#else
#define VEE_PLATFORM_X32 1
#define VEE_PLATFORM_X64 0
#endif
#endif

#if (_MSC_VER == 1900) // VS2015
#define __noexcept noexcept
#define __constexpr constexpr
#else
#define __noexcept throw()
#define __constexpr
#endif
    
} // !namespace vee

#endif // !_VEE_PLATFORM_H_