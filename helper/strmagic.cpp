#include <vee/helper/strmagic.h>
#include <boost/tokenizer.hpp>

namespace vee {

namespace strmagic {


std::vector<::std::string> split(const ::std::string& string, const char* token)
{
    using char_separator = ::boost::char_separator<char>;
    using tokenizer = boost::tokenizer<char_separator>;

    // Parse raw data per lines (token: \n)
    ::std::vector<::std::string> ret;
    {
        char_separator sep(token);
        tokenizer tok(string, sep);
        for (auto& it : tok)
        {
            ret.push_back(it);
        }
    }

    return ret;
}

} // !namespace strmagic

} // !namespace vee