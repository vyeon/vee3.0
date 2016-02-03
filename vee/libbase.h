#ifndef _VEE_LIBBASE_H_
#define _VEE_LIBBASE_H_

#define _VEE    ::vee::
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&) = delete; \
void operator=(const TypeName&) = delete;
#define DISALLOW_MOVE_AND_ASSIGN(TypeName) \
TypeName(const TypeName&&) = delete; \
void operator=(const TypeName&&) = delete;
#define VEE_EMPTY_FUNCTION {}
#ifndef _VEE_ENABLE_LOGGING
#define _VEE_ENABLE_LOGGING 1
#endif
#ifdef _VEE_ENABLE_LOGGING
#define DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define DEBUG_PRINT(...)
#endif

// Check windows
#if _WIN32 || _WIN64
#define VEE_PLATFORM_WINDOWS 1
#define _WIN32_WINNT 0x0501
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

#define __PURE = 0

#define PRINT_LINE for(int i = 0; i < 7; ++i) printf("----------");printf("\n");

#if (_MSC_VER == 1900) // VS2015
#define __noexcept noexcept
#define __constexpr constexpr
#else
#define __noexcept throw()
#define __constexpr
#endif

#endif // !_VEE_LIBBASE_H_
