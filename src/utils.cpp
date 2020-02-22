#include "utils.h"
#include <cstdlib>

namespace configparser
{
namespace detail
{

bool parse_integer_base(number& num, const char* text, std::ptrdiff_t length, int base)
{
    char* end_ptr = nullptr;
    num.nb.l = std::strtol(text, &end_ptr, base);
    num.type = NumberType::NUMBER_LONG;

    return end_ptr == (text + length);
}

std::string remove_escapes(std::string&& str)
{
    const bool has_escapes = std::find(str.begin(), str.end(), '\\') != str.end();
    if (has_escapes)
    {
        size_t i = 0;
        size_t num_escapes = 0;
        while ((str[i] == ' ') || (str[i] == '\\'))
        {
            if (str[i] == '\\')
            {
                ++num_escapes;
                str[i] = ' ';
            }

            ++i;
        }

        str.assign(str.begin() + num_escapes, str.end());
        if (num_escapes < str.size())
        {
            i = str.size() - 1;
            num_escapes = 0;
            while ((str[i] == ' ') || (str[i] == '\\'))
            {
                if (str[i] == '\\')
                {
                    ++num_escapes;
                    str[i] = ' ';
                }

                --i;
            }

            str.assign(str.begin(), str.end() - num_escapes);
        }
    }

    return std::move(str);
}

} // detail
} // configparser
