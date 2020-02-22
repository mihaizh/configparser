#ifndef CP_TOKENIZER_H
#define CP_TOKENIZER_H

#include "error_code.h"
#include "token.h"
#include <vector>

namespace configparser
{
namespace detail
{
    class tokenizer
    {
    public:
        tokenizer() = default;

        ErrorCode parse(const char* text);

        const std::vector<token>& tokens() const;

        int current_line() const;
        int current_column() const;

    private:
        char peek() const;
        char peekNext() const;
        void consume();

        bool eof() const;
        bool eol() const;
        bool empty() const;

        static bool priority_value_separator(char val);
        static bool value_separator(char val);

        void comment();

        bool is_identifier_start();
        bool is_identifier_char();

        void identifier();
        void section();
        void value();

        const char* m_text_ptr = nullptr;
        std::vector<token> m_tokens;
        ErrorCode m_error_code;

        int m_line;
        int m_column;
    }; // tokenizer
} // detail
} // configparser

#endif // CP_TOKENIZER_H
