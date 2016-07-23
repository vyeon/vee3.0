#ifndef _VEE_LIBBASE_H_
#define _VEE_LIBBASE_H_

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

#define PRINT_LINE for(int i = 0; i < 7; ++i) printf("----------");printf("\n");

#endif // !_VEE_LIBBASE_H_
