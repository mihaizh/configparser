#ifndef CP_TOKEN_H
#define CP_TOKEN_H

#include <cstddef>

namespace configparser
{
namespace detail
{

    enum class TokenType
    {
        TOKEN_SECTION,
        TOKEN_IDENTIFIER,
        TOKEN_VALUE,
        TOKEN_VECTOR_VALUE,

        TOKEN_NUM
    }; // TokenType

    struct token
    {
        TokenType type;
        const char* begin_ptr;
        std::ptrdiff_t length;

        int line;
        int column;
    }; // token

} // detail
} // tdetail

#endif // CP_TOKEN_H
