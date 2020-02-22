#include "value_parser.h"
#include <memory>

namespace configparser
{
namespace detail
{

bool link_parser::is_valid(const char* text, std::ptrdiff_t length)
{
    // a link should contain at least:
    // $, {, #, }, one char for section, one char for option
    // e.g.: ${AnySection#MyOption}
    const bool valid_length = length >= 6; 
    if (valid_length)
    {
        const bool valid_start =
            (*text == '$') &&
            (*(text + 1) == '{');

        const bool valid_end =
            (*(text + length - 1) == '}');

        return valid_start && valid_end;
    }

    return false;
}

bool link_parser::parse(const char* text, std::ptrdiff_t length)
{
    if (!is_valid(text, length))
    {
        return false;
    }

    const char* begin_ptr = text + 2; // skip '${'
    const char* end_ptr = text + length - 1; // skip '}'

    const char* section_begin_ptr = begin_ptr;

    // consume everything until option separator
    while ((begin_ptr < end_ptr) && (*begin_ptr != '#'))
    {
        ++begin_ptr;
    }

    const std::ptrdiff_t section_length = begin_ptr - text - 2;

    const char* option_begin_ptr = begin_ptr + 1;
    const std::ptrdiff_t option_length = end_ptr - option_begin_ptr;

    m_section.assign(section_begin_ptr, section_length);
    m_option.assign(option_begin_ptr, option_length);

    return !m_section.empty() && !m_option.empty();
}

const std::string& link_parser::section() const
{
    return m_section;
}

const std::string& link_parser::option() const
{
    return m_option;
}

bool number_parser::is_hexadecimal(const char* text, std::ptrdiff_t length)
{
    // e.g.: 0xA1B2C3
    return (length > 2) && // '0x'
        (*text == '0') &&
        (*(text + 1) == 'x');
}

bool number_parser::is_decimal(const char* text, std::ptrdiff_t length)
{
    // any type of number
    return isdigit(*text) ||
        (*text == '+') ||
        (*text == '-');
}

bool number_parser::is_octal(const char* text, std::ptrdiff_t length)
{
    // e.g.: 01337
    return (length > 1) && // '0'
        (*text == '0');
}

bool number_parser::is_binary(const char* text, std::ptrdiff_t length)
{
    // e.g.: 0b00101010
    return (length > 2) && // '0b'
        (*text == '0') &&
        (*(text + 1) == 'b');
}

number_parser::InternalNumberType number_parser::get_number_type(const char* text, std::ptrdiff_t length)
{
    const char* begin_ptr = text;
    const char* end_ptr = text + length - 1;

    if (is_hexadecimal(text, length))
    {
        begin_ptr += 2; // skip '0x'
        static const char* special_chars = "ABCDEFabcdef";

        // consume all characters, but if a character
        // is not a digit or a part of special characters
        // then it is not a valid hexadecimal number
        while (begin_ptr < end_ptr)
        {
            if (!isdigit(*begin_ptr) && !strchr(special_chars, *begin_ptr))
            {
                return InternalNumberType::NUMBER_NUM;
            }

            ++begin_ptr;
        }

        return InternalNumberType::NUMBER_HEXADECIMAL;
    }
    else if (is_binary(text, length))
    {
        begin_ptr += 2; // skip '0b'

        // a binary number should have
        // only '1's and '0's
        while (begin_ptr < end_ptr)
        {
            if ((*begin_ptr != '0') && (*begin_ptr != '1'))
            {
                return InternalNumberType::NUMBER_NUM;
            }

            ++begin_ptr;
        }

        return InternalNumberType::NUMBER_BINARY;
    }
    else if (is_octal(text, length))
    {
        static const char* valid_characters = "01234567";

        // only digits from 0 to 7 are valid
        while (begin_ptr < end_ptr)
        {
            if (!strchr(valid_characters, *begin_ptr))
            {
                return InternalNumberType::NUMBER_NUM;
            }

            ++begin_ptr;
        }

        return InternalNumberType::NUMBER_OCTAL;
    }
    else if (is_decimal(text, length))
    {
        // a decimal number can start with '+' or '-'
        if ((*text == '+') || (*text == '-'))
        {
            begin_ptr += 1; // skip '+' or '-'
        }

        // consume all available digits
        while ((begin_ptr < end_ptr) && isdigit(*begin_ptr))
        {
            ++begin_ptr;
        }

        // if a dot is encountered, check if it's
        // a valid floating point number
        if (*begin_ptr == '.')
        {
            // consume dot
            ++begin_ptr;

            // consume all available digits
            while ((begin_ptr < end_ptr) && isdigit(*begin_ptr))
            {
                ++begin_ptr;
            }

            // if 'E' or 'e' is encountered, then
            // it's still a valid number so the check continues
            if ((*begin_ptr == 'E') || (*begin_ptr == 'e'))
            {
                ++begin_ptr;

                // check if 'E' or 'e' is followed
                // by '+' or '-' and consume it
                if ((*begin_ptr == '+') ||
                    (*begin_ptr == '-'))
                {
                    ++begin_ptr;
                }

                // after 'E', 'e', '+' or '-' at least
                // one more digit is required
                if (!isdigit(*begin_ptr))
                {
                    return InternalNumberType::NUMBER_NUM;
                }

                // continue consuming digits until the end
                while ((begin_ptr < end_ptr) && (isdigit(*begin_ptr)))
                {
                    ++begin_ptr;
                }
            }

            // if another non-digit character is encountered
            // then it's an unknown number format
            return (begin_ptr < end_ptr)
                ? InternalNumberType::NUMBER_NUM
                : InternalNumberType::NUMBER_DOUBLE;
        }

        // no dot encountered, so it can be an integer
        return (begin_ptr < end_ptr)
            ? InternalNumberType::NUMBER_NUM
            : InternalNumberType::NUMBER_INTEGER;
    }

    // unknown number format
    return InternalNumberType::NUMBER_NUM;
}

bool number_parser::parse_binary(const char* text, std::ptrdiff_t length)
{
    return parse_integer_base(m_number, text, length, 2);
}

bool number_parser::parse_hexadecimal(const char* text, std::ptrdiff_t length)
{
    return parse_integer_base(m_number, text, length, 16);
}

bool number_parser::parse_integer(const char* text, std::ptrdiff_t length)
{
    return parse_integer_base(m_number, text, length, 10);
}

bool number_parser::parse_octal(const char* text, std::ptrdiff_t length)
{
    return parse_integer_base(m_number, text, length, 8);
}

bool number_parser::parse_double(const char* text, std::ptrdiff_t length)
{
    // let everything to std?
    char* end_ptr = nullptr;
    m_number.nb.d = std::strtod(text, &end_ptr);
    m_number.type = NumberType::NUMBER_DOUBLE;

    return end_ptr == (text + length);
}

bool number_parser::parse(const char* text, std::ptrdiff_t length)
{
    const auto number_type = get_number_type(text, length);
    switch (number_type)
    {
        case InternalNumberType::NUMBER_BINARY:
            return parse_binary(text + 2, length - 2);
        case InternalNumberType::NUMBER_HEXADECIMAL:
            return parse_hexadecimal(text, length);
        case InternalNumberType::NUMBER_OCTAL:
            return parse_octal(text, length);
        case InternalNumberType::NUMBER_INTEGER:
            return parse_integer(text, length);
        case InternalNumberType::NUMBER_DOUBLE:
            return parse_double(text, length);
        default:
            return false;
    }
}

const number& number_parser::get() const
{
    return m_number;
}

bool boolean_parser::is_true(const char* text, std::ptrdiff_t length)
{
    if (length == 1)
    {
        return (*text == 'y') ||
            (*text == 't');
    }
    else
    {
        if (_strcmpi("on", text) == 0) return true;
        if (_strcmpi("yes", text) == 0) return true;
        if (_strcmpi("enabled", text) == 0) return true;
    }

    return false;
}

bool boolean_parser::is_false(const char* text, std::ptrdiff_t length)
{
    if (length == 1)
    {
        return (*text == 'n') ||
            (*text == 'f');
    }
    else
    {
        if (_strcmpi("off", text) == 0) return true;
        if (_strcmpi("no", text) == 0) return true;
        if (_strcmpi("disabled", text) == 0) return true;
    }

    return false;
}

bool boolean_parser::parse(const char* text, std::ptrdiff_t length)
{
    std::string str;
    str.reserve(length);
    str.assign(text, length);

    m_boolean = is_true(str.c_str(), length);

    return m_boolean || is_false(str.c_str(), length);
}

bool boolean_parser::get() const
{
    return m_boolean;
}

} // detail
} // configparser
