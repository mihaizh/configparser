#include "configparser.h"
#include "value_parser.h"
#include "tokenizer.h"

#include <fstream> // ifstream
#include <cassert> // assert

namespace configparser
{

void ConfigParser::parse_value(values_vector& values, const detail::token& t)
{
    detail::link_parser lp;
    if (lp.parse(t.begin_ptr, t.length))
    {
        const auto it = m_sections_map.find(lp.section());
        if (it != m_sections_map.end())
        {
            const auto& scts = m_sections[it->second];
            const auto it2 = scts.m_options_map.find(lp.option());
            if (it2 != scts.m_options_map.end())
            {
                const values_vector& other_values = scts.m_options[it2->second].m_values;
                std::copy(other_values.begin(), other_values.end(), std::back_inserter(values));
                return;
            }
        }
    }

    detail::boolean_parser bp;
    if (bp.parse(t.begin_ptr, t.length))
    {
        values.emplace_back(bp.get());
        return;
    }

    detail::number_parser np;
    if (np.parse(t.begin_ptr, t.length))
    {
        switch (np.get().type)
        {
            case detail::NumberType::NUMBER_LONG:
                values.emplace_back(np.get().nb.l);
                return;
            case detail::NumberType::NUMBER_DOUBLE:
                values.emplace_back(np.get().nb.d);
                return;
            default: // should NEVER get here
                assert(false);
                break;
        }
    }

    values.emplace_back(detail::remove_escapes(std::string{ t.begin_ptr, (size_t)t.length }));
}

bool ConfigParser::parse_tokens(const std::vector<detail::token>& tokens)
{
    auto it = tokens.begin();
    if (it->type != detail::TokenType::TOKEN_SECTION)
    {
        m_error_code = ErrorCode::EXPECTED_SECTION_FIRST;
        m_error_line = it->line;
        m_error_column = it->column;
        return false;
    }

    m_sections_map.emplace(
        std::string{ it->begin_ptr, (size_t)it->length },
        m_sections.size());

    m_sections.emplace_back(
        std::string{ it->begin_ptr, (size_t)it->length });

    for (++it; it != tokens.end(); ++it)
    {
        switch (it->type)
        {
            case detail::TokenType::TOKEN_SECTION:
            {
                m_sections_map.emplace(
                    std::string{ it->begin_ptr, (size_t)it->length },
                    m_sections.size());

                m_sections.emplace_back(
                    std::string{ it->begin_ptr, (size_t)it->length });
            }
            break;
            case detail::TokenType::TOKEN_IDENTIFIER:
            {
                m_sections.back().m_options_map.emplace(
                    std::string{ it->begin_ptr, (size_t)it->length },
                    m_sections.back().m_options.size());

                m_sections.back().m_options.emplace_back(
                    std::string{ it->begin_ptr, (size_t)it->length });

                const auto identifier_it = it++;
                if (it == tokens.end())
                {
                    m_error_code = ErrorCode::EXPECTING_VALUE_AFTER_IDENTIFIER;
                    m_error_line = identifier_it->line;
                    m_error_column = identifier_it->column;
                    return false;
                }

                values_vector& values = m_sections.back().m_options.back().m_values;
                if (it->type == detail::TokenType::TOKEN_VALUE)
                {
                    parse_value(values, *it);
                }
                else if (it->type == detail::TokenType::TOKEN_VECTOR_VALUE)
                {
                    while ((it != tokens.end()) && (it->type == detail::TokenType::TOKEN_VECTOR_VALUE))
                    {
                        parse_value(values, *it);
                        ++it;
                    }

                    --it;
                }
                else
                {
                    m_error_code = ErrorCode::EXPECTING_VALUE_AFTER_IDENTIFIER;
                    m_error_line = identifier_it->line;
                    m_error_column = identifier_it->column;
                    return false;
                }
            }
            break;
            case detail::TokenType::TOKEN_VALUE:
            case detail::TokenType::TOKEN_VECTOR_VALUE:
            {
                m_error_code = ErrorCode::UNEXPECTED_VALUE;
                m_error_line = it->line;
                m_error_column = it->column;
                return false;
            }
            break;
            default:
            {
                m_error_code = ErrorCode::UNEXPECTED_TOKEN;
                m_error_line = it->line;
                m_error_column = it->column;
                return false;
            }
            break;
        }
    }

    return true;
}

bool ConfigParser::parse_text(const char* text)
{
    m_sections.clear();
    m_sections_map.clear();

    detail::tokenizer t;
    m_error_code = t.parse(text);

    if (m_error_code != ErrorCode::NO_ERROR)
    {
        m_error_line = t.current_line();
        m_error_column = t.current_column();

        return false;
    }

    if (!t.tokens().empty())
    {
        return parse_tokens(t.tokens());
    }

    return m_error_code == ErrorCode::NO_ERROR;
}

bool ConfigParser::parse_file(const char* filename)
{
    std::ifstream f(filename);
    if (f.is_open())
    {
        std::string str;
        f.seekg(0, std::ios::end);
        str.reserve(f.tellg());
        f.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(f)),
            std::istreambuf_iterator<char>());

        return parse_text(str.c_str());
    }

    return false;
}

ErrorCode ConfigParser::error_code() const
{
    return m_error_code;
}

int ConfigParser::get_error_line() const
{
    return m_error_line;
}

int ConfigParser::get_error_column() const
{
    return m_error_column;
}

const section_vector& ConfigParser::sections() const
{
    return m_sections;
}

const option_vector& ConfigParser::options(const std::string& section_name) const
{
    return m_sections[m_sections_map.at(section_name)].options();
}

const option_type& ConfigParser::option(const std::string& section_name, const std::string& option_name) const
{
    return m_sections[m_sections_map.at(section_name)].option(option_name);
}

bool ConfigParser::has_section(const std::string& section_name) const
{
    return m_sections_map.find(section_name) != m_sections_map.end();
}

bool ConfigParser::has_option(const std::string& section_name, const std::string& option_name) const
{
    const auto it = m_sections_map.find(section_name);
    return ((it != m_sections_map.end())) &&
        (m_sections[it->second].has_option(option_name));
}

} // configparser
