#ifndef CP_SECTION_TYPE_H
#define CP_SECTION_TYPE_H

#include "option_type.h"
#include <unordered_map>

namespace configparser
{

    class section_type
    {
    public:
        section_type(std::string&& name) noexcept;
        section_type(const section_type&) = default;
        section_type(section_type&&) noexcept = default;
        section_type& operator=(const section_type&) = default;
        section_type& operator=(section_type&&) noexcept = default;
        ~section_type() = default;

        const option_vector& options() const;
        const option_type& option(const std::string& option_name) const;

        bool has_option(const std::string& option_name) const;

        const std::string& name() const;

    private:
        using option_map = std::unordered_map<std::string, size_t>;

        std::string m_name;
        option_vector m_options;
        option_map m_options_map;

        friend class ConfigParser;
    }; // section_type

    using section_vector = std::vector<section_type>;

} // configparser

#endif // CP_SECTION_TYPE_H
