#include <vee/helper/term.h>
#include <cstdio>

namespace vee {

void print_hexa(uint8_t* buffer, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        printf("0x%02X ", buffer[i]);
    }
}

} // !namespace vee