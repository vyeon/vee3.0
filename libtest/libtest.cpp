#include <vee/libtest.h>
#include <cstdio>

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#endif // !_WIN32

namespace {

using color_t = short;

bool set_color(color_t color)
{
#ifdef _WIN32
    return !!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else 
    return true;
#endif // !_WIN32
};

bool get_color(color_t &ret)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return false;
    ret = info.wAttributes;
    return true;
#else
    return true;
#endif // !_WIN32
};

} // !unnamed namespace

namespace vee {

namespace libtest {

void test_log(bool result, const char* test_name, const char* detail, ...) noexcept
{
    char buffer[512] = { 0, };
    va_list ap;
    va_start(ap, detail);
    vsprintf_s(buffer, detail, ap);
    va_end(ap);

    if (result)
        test_success(test_name, buffer);
    else
        test_failed(test_name, buffer);
}

void test_success(const char* test_name, const char* detail) noexcept
{
    color_t cdef_origin;
    if (!get_color(cdef_origin))
        cdef_origin = 0x0A;

    set_color(0x0F);
    printf("@ libtest");
    set_color(0x03);
    printf(" %s\n", test_name); 
    set_color(0x0F);
    printf("  result:");
    set_color(0x0A);
    printf(" Success");

    set_color(cdef_origin);
    printf("\n  detail: %s\n", detail);
}

void test_failed(const char* test_name, const char* detail) noexcept
{
    color_t cdef_origin;
    if (!get_color(cdef_origin))
        cdef_origin = 0x07;

    set_color(0x0F);
    printf("@ libtest");
    set_color(0x03);
    printf(" %s\n", test_name);
    set_color(0x0F);
    printf("  result:");
    set_color(0x0C);
    printf(" Failed");

    set_color(cdef_origin);
    printf("\n  detail: %s\n", detail);
}

} // !namespace libtest

} // !namespace vee