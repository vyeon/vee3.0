#ifndef _VEE_HELPER_STRMAGIC_H_
#define _VEE_HELPER_STRMAGIC_H_

#include <locale>
#include <vector>
#include <algorithm>

namespace vee {

namespace strmagic {

template<typename charT>
struct equal
{
    equal(const ::std::locale& loc): loc_(loc)
    {
    }
    bool operator()(charT ch1, charT ch2)
    {
        return ::std::toupper(ch1, loc_) == ::std::toupper(ch2, loc_);
    }
private:
    const ::std::locale& loc_;
};

inline ::std::string trim_left(const ::std::string& str)
{
    ::std::string::size_type n = str.find_first_not_of(" \t\v\n");
    return n == ::std::string::npos ? str : str.substr(n, str.length());
}

inline ::std::string trim_right(const ::std::string& str)
{
    ::std::string::size_type n = str.find_last_not_of(" \t\v\n");
    return n == ::std::string::npos ? str : str.substr(0, n + 1);
}

inline ::std::string trim(const ::std::string& str)
{
    return trim_left(trim_right(str));
}

std::vector<::std::string> split(const ::std::string& string, const char* token);

const int not_found = -1;

template<typename T>
static int ci_find_substr(const T& str1, const T& str2, const ::std::locale& loc = ::std::locale())
{
    typename T::const_iterator it = ::std::search(str1.begin(), str1.end(),
                                                  str2.begin(), str2.end(), equal<typename T::value_type>(loc));
    if (it != str1.end()) return it - str1.begin();
    else return not_found; // not found
}

char* stristr(const char* s1, const char* s2);

} // !namespace strmagic

} // !namespace vee

#endif // !_VEE_HELPER_STRMAGIC_H_