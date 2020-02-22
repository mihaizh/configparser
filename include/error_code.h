#ifndef CP_ERROR_CODE_H
#define CP_ERROR_CODE_H

namespace configparser
{
    enum class ErrorCode
    {
        NO_ERROR,

        // tokenizer error codes
        UNEXPECTED_CHARACTER,
        EXPECTED_CLOSING_BRACKET,

        // parser error codes
        EXPECTED_SECTION_FIRST,
        EXPECTING_VALUE_AFTER_IDENTIFIER,
        UNEXPECTED_VALUE,
        UNEXPECTED_TOKEN,
    }; // ErrorCode
}

#endif // CP_ERROR_CODE_H
