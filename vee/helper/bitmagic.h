#pragma once
#ifndef _VEE_HELPER_BITMAGIC_H_
#define _VEE_HELPER_BITMAGIC_H_

#include <string>

namespace vee {

template <class T>
inline uint32_t getbit(T x, uint32_t n)
{
    return (x & (1 << n)) >> n;
}

template <class T>
std::string to_bit_string(T&& arg)
{
    std::string str{};
    for (uint32_t i = 0; i < sizeof(T); ++i)
    {
        uint8_t c = 0;
        char buf[10] = { 0, };
        c |= *(reinterpret_cast<uint8_t*>(&arg) + i);
        for (uint32_t j = 0; j < 8; ++j)
        {
            buf[j] = getbit(c, j) + 48;
        }
        buf[8] = ' ';
        str.append(buf);
    }
    return str;
}

} // !namespace vee

#endif // !_VEE_HELPER_BITMAGIC_H_