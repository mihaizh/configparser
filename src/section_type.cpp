#include "section_type.h"

namespace configparser
{

const option_vector& section_type::options() const
{
    return m_options;
}

const option_type& section_type::option(const std::string& option_name) const
{
    return m_options[m_options_map.at(option_name)];
}

section_type::section_type(std::string&& name) noexcept
    : m_name(std::move(name))
{
}

bool section_type::has_option(const std::string& option_name) const
{
    return m_options_map.find(option_name) != m_options_map.end();
}

const std::string& section_type::name() const
{
    return m_name;
}

} // configparser
