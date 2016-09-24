#ifndef _VEE_HELPER_TERM_H_
#define _VEE_HELPER_TERM_H_

#include <cstdint>
#include <cstdio>
#include <vee/helper/bitmagic.h>

namespace vee {

void print_hexa(uint8_t* buffer, size_t size);

template <class T>
void print_bits(T&& value)
{
    for (uint32_t i = 0; i < sizeof(T); ++i)
    {
        uint8_t c = 0;
        c |= *(reinterpret_cast<uint8_t*>(&value) + i);
        for (uint32_t j = 0; j < 8; ++j)
        {
            printf("%u", getbit(c, j));
        }
        printf(" ");
    }
}

template <class T, int PRINT_CARRIAGE_RETURN_THRESHOLD = 16>
void print_bytes(T&& value)
{
    for (size_t i = 1; i <= sizeof(T); ++i)
    {
        printf("0x%02X ", *(reinterpret_cast<uint8_t*>(&value) + (i - 1)));
        if (i % PRINT_CARRIAGE_RETURN_THRESHOLD == 0)
            printf("\n");
    }
}

template <class T, int PRINT_CARRIAGE_RETURN_THRESHOLD = 16>
void print_bytes_reverse(T&& value)
{
    size_t cnt = 0;
    for (size_t i = sizeof(T); i > 0; --i)
    {
        printf("0x%02X ", *(reinterpret_cast<uint8_t*>(&value) + (i - 1)));
        if (cnt++ % PRINT_CARRIAGE_RETURN_THRESHOLD == 0)
            printf("\n");
    }
}

} // !namespace vee

#endif // !_VEE_HELPER_TERM_H_