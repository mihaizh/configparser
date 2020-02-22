#ifndef CP_UTILS_H
#define CP_UTILS_H

#include <cstddef>
#include <memory>
#include <string>

namespace configparser
{
namespace detail
{
    enum class NumberType
    {
        NUMBER_LONG,
        NUMBER_DOUBLE,

        NUMBER_NUM
    }; // TokenType

    union number_union
    {
        long l;
        double d;
    }; // number_union

    struct number
    {
        NumberType type;
        number_union nb;
    }; // number

    bool parse_integer_base(number& num, const char* text, std::ptrdiff_t length, int base);
    std::string remove_escapes(std::string&& str);
} // detail
} // configparser

#endif // CP_UTILS_H
