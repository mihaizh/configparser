#ifndef CP_VALUE_PARSER_H
#define CP_VALUE_PARSER_H

#include "utils.h"
#include <string>

namespace configparser
{
namespace detail
{
    class link_parser
    {
    public:
        link_parser() = default;
        link_parser(const link_parser&) = default;
        link_parser(link_parser&&) noexcept = default;
        link_parser& operator=(const link_parser&) = default;
        link_parser& operator=(link_parser&&) = default;
        ~link_parser() = default;

        bool parse(const char* text, std::ptrdiff_t length);

        const std::string& section() const;
        const std::string& option() const;

    private:
        static bool is_valid(const char* text, std::ptrdiff_t length);

        std::string m_section;
        std::string m_option;
    }; // link_parser

    class number_parser
    {
    public:
        number_parser() = default;
        number_parser(const number_parser&) = default;
        number_parser(number_parser&&) noexcept = default;
        number_parser& operator=(const number_parser&) = default;
        number_parser& operator=(number_parser&&) = default;
        ~number_parser() = default;

        bool parse(const char* text, std::ptrdiff_t length);
        const number& get() const;

    private:
        enum class InternalNumberType
        {
            NUMBER_HEXADECIMAL,
            NUMBER_OCTAL,
            NUMBER_BINARY,
            NUMBER_INTEGER,
            NUMBER_DOUBLE,

            NUMBER_NUM
        }; // InternalNumberType

        static bool is_hexadecimal(const char* text, std::ptrdiff_t length);
        static bool is_decimal(const char* text, std::ptrdiff_t length);
        static bool is_octal(const char* text, std::ptrdiff_t length);
        static bool is_binary(const char* text, std::ptrdiff_t length);

        static InternalNumberType get_number_type(const char* text, std::ptrdiff_t length);

        bool parse_hexadecimal(const char* text, std::ptrdiff_t length);
        bool parse_octal(const char* text, std::ptrdiff_t length);
        bool parse_binary(const char* text, std::ptrdiff_t length);
        bool parse_integer(const char* text, std::ptrdiff_t length);
        bool parse_double(const char* text, std::ptrdiff_t length);

        number m_number;
    }; // number_parser

    class boolean_parser
    {
    public:
        boolean_parser() = default;
        boolean_parser(const boolean_parser&) = default;
        boolean_parser(boolean_parser&&) noexcept = default;
        boolean_parser& operator=(const boolean_parser&) = default;
        boolean_parser& operator=(boolean_parser&&) = default;
        ~boolean_parser() = default;

        bool parse(const char* text, std::ptrdiff_t length);
        bool get() const;

    private:
        static bool is_true(const char* text, std::ptrdiff_t length);
        static bool is_false(const char* text, std::ptrdiff_t length);

        bool m_boolean;
    }; // boolean_parser
} // detail
} // configparser

#endif // CP_VALUE_PARSER_H
