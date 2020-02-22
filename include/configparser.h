#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "error_code.h"
#include "section_type.h"
#include "token.h"

namespace configparser
{

    class ConfigParser
    {
    public:
        ConfigParser() = default;
        ConfigParser(const ConfigParser&) = default;
        ConfigParser(ConfigParser&&) noexcept = default;
        ConfigParser& operator=(const ConfigParser&) = default;
        ConfigParser& operator=(ConfigParser&&) noexcept = default;
        ~ConfigParser() = default;

        bool parse_text(const char* text);
        bool parse_file(const char* filename);

        ErrorCode error_code() const;
        int get_error_line() const;
        int get_error_column() const;

        const section_vector& sections() const;
        const option_vector& options(const std::string& section_name) const;
        const option_type& option(const std::string& section_name, const std::string& option_name) const;

        bool has_section(const std::string& section_name) const;
        bool has_option(const std::string& section_name, const std::string& option_name) const;

    public:
        using section_map = std::unordered_map<std::string, size_t>;

        bool parse_tokens(const std::vector<detail::token>& tokens);
        void parse_value(values_vector& values, const detail::token& t);

        section_vector m_sections;
        section_map m_sections_map;

        ErrorCode m_error_code;
        int m_error_line;
        int m_error_column;
    }; // ConfigParser

} // configparser

#endif // CONFIGPARSER_H
