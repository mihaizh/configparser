#include "tokenizer.h"
#include <cctype> // isalpha, isdigit

namespace configparser
{
namespace detail
{

char tokenizer::peek() const
{
    return *m_text_ptr;
}

char tokenizer::peekNext() const
{
    if (eof())
    {
        return 0;
    }

    return *(m_text_ptr + 1);
}

void tokenizer::consume()
{
    ++m_text_ptr;
    ++m_column;
}

bool tokenizer::eof() const
{
    return peek() == 0;
}

bool tokenizer::eol() const
{
    return peek() == '\n';
}

bool tokenizer::empty() const
{
    return (peek() == ' ') ||
        (peek() == '\t');
}

void tokenizer::comment()
{
    while (peek() != '\n')
    {
        consume();
    }
}

bool tokenizer::is_identifier_start()
{
    const bool escaped_start =
        (peek() == '\\') &&
        (peekNext() == ' ');

    return escaped_start ||
        isalpha(peek()) ||
        (peek() == '.') ||
        (peek() == '$') ||
        (peek() == ':');
}

bool tokenizer::is_identifier_char()
{
    return is_identifier_start() ||
        isdigit(peek()) ||
        (peek() == '_') ||
        (peek() == '~') ||
        (peek() == '-') ||
        (peek() == '.') ||
        (peek() == ':') ||
        (peek() == ' ');
}

bool tokenizer::priority_value_separator(char val)
{
    return val == ',';
}

bool tokenizer::value_separator(char val)
{
    return (val == ':') ||
        (val == ',');
}

void tokenizer::identifier()
{
    const char* begin_ptr = m_text_ptr;
    while (is_identifier_char())
    {
        consume();
    }

    const char* end_ptr = m_text_ptr - 1;

    // strip whitespace at the end of identifier
    while (*end_ptr == ' ')
    {
        --end_ptr;
    }

    m_tokens.emplace_back();
    m_tokens.back().type = TokenType::TOKEN_IDENTIFIER;
    m_tokens.back().begin_ptr = begin_ptr;
    m_tokens.back().length = end_ptr - begin_ptr + 1;
    m_tokens.back().line = m_line;
    m_tokens.back().column = m_column - 1;
}

void tokenizer::section()
{
    consume(); // '['

    const char* begin_ptr = m_text_ptr;
    while (is_identifier_char())
    {
        consume();
    }

    // check for bracket at the end of
    // section define
    if (peek() != ']')
    {
        m_error_code = ErrorCode::EXPECTED_CLOSING_BRACKET;
        --m_column;
    }
    else
    {
        m_tokens.emplace_back();
        m_tokens.back().type = TokenType::TOKEN_SECTION;
        m_tokens.back().begin_ptr = begin_ptr;
        m_tokens.back().length = m_text_ptr - begin_ptr;
        m_tokens.back().line = m_line;
        m_tokens.back().column = m_column - (int)m_tokens.back().length;
        consume(); // ']'
    }
}

void tokenizer::value()
{
    consume(); // '='

    // strip whitespace at the beginning
    // of a value
    while (empty())
    {
        consume();
    }

    const char* begin_ptr = m_text_ptr;
    char sep = 0; // save items separator, if exists
    while (!eof() && !eol() && (peek() != ';'))
    {
        // save the items separator, with higher priority
        // to one of them
        if (!priority_value_separator(sep) &&
            value_separator(peek()))
        {
            sep = peek();
        }

        consume();
    }

    const TokenType token_type = (sep == 0) ?
        TokenType::TOKEN_VALUE :
        TokenType::TOKEN_VECTOR_VALUE;

    const char* current_ptr = begin_ptr;
    while (current_ptr < m_text_ptr)
    {
        // consume everything until items separator
        while ((current_ptr < m_text_ptr) && (*current_ptr != sep))
        {
            ++current_ptr;
        }

        const char* end_ptr = current_ptr - 1;

        // if there's a comment at the end of the line
        // then strip the comment and any unescaped whitespace
        if (*current_ptr == ';')
        {
            while ((*end_ptr == ' ') || (*end_ptr == '\t'))
            {
                --end_ptr;
                if (*end_ptr == '\\')
                {
                    ++end_ptr;
                    break;
                }
            }
        }

        m_tokens.emplace_back();
        m_tokens.back().type = token_type;
        m_tokens.back().begin_ptr = begin_ptr;
        m_tokens.back().length = end_ptr - begin_ptr + 1;
        m_tokens.back().line = m_line;
        m_tokens.back().column = m_column - (int)(m_text_ptr - begin_ptr);

        // if a comment follows the value, then
        // no more parsing is needed
        if (*current_ptr == ';')
        {
            break;
        }

        // skip the items separator and strip
        // any unescaped whitespace before the next value
        if (*current_ptr == sep)
        {
            ++current_ptr;
            while ((*current_ptr == ' ') || (*current_ptr == '\t'))
            {
                ++current_ptr;
            }
        }

        begin_ptr = current_ptr;
        ++current_ptr;
    }
}

ErrorCode tokenizer::parse(const char* text)
{
    m_tokens.clear();
    m_text_ptr = text;
    m_error_code = ErrorCode::NO_ERROR;

    m_line = 1;
    m_column = 1;

    while (!eof() && (m_error_code == ErrorCode::NO_ERROR))
    {
        if (is_identifier_start())
        {
            identifier();
        }

        switch (peek())
        {
            case ' ':
            case '\t':
            case '\r':
                consume();
                break;
            case '\n':
                ++m_line;
                m_column = 1;
                consume();
                break;
            case ';':
                comment();
                break;
            case '[':
                section();
                break;
            case '=':
                value();
                break;
            default: // error
                m_error_code = ErrorCode::UNEXPECTED_CHARACTER;
                break;
        }
    }

    return m_error_code;
}

const std::vector<token>& tokenizer::tokens() const
{
    return m_tokens;
}

int tokenizer::current_line() const
{
    return m_line;
}

int tokenizer::current_column() const
{
    return m_column;
}

} // detail
} // configparser
