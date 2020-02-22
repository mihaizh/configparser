#include "option_type.h"

namespace configparser
{

option_type::option_type(std::string&& name) noexcept
    : m_name(std::move(name))
{
}

template <>
long option_type::get<long>(size_t idx) const
{
    return m_values[idx].to_long();
}

template <>
double option_type::get<double>(size_t idx) const
{
    return m_values[idx].to_double();
}

template <>
bool option_type::get<bool>(size_t idx) const
{
    return m_values[idx].to_bool();
}

template <>
const std::string& option_type::get<const std::string&>(size_t idx) const
{
    return m_values[idx].to_str();
}

template <>
bool option_type::get<long>(size_t idx, long& val) const
{
    if ((idx < m_values.size()) &&
        (m_values[idx].has_type(ValueType::VALUE_LONG)))
    {
        val = m_values[idx].to_long();
        return true;
    }

    return false;
}

template <>
bool option_type::get<double>(size_t idx, double& val) const
{
    if ((idx < m_values.size()) &&
        (m_values[idx].has_type(ValueType::VALUE_DOUBLE)))
    {
        val = m_values[idx].to_double();
        return true;
    }

    return false;
}

template <>
bool option_type::get<bool>(size_t idx, bool& val) const
{
    if ((idx < m_values.size()) &&
        (m_values[idx].has_type(ValueType::VALUE_BOOLEAN)))
    {
        val = m_values[idx].to_bool();
        return true;
    }

    return false;
}

template <>
bool option_type::get<std::string>(size_t idx, std::string& val) const
{
    if ((idx < m_values.size()) &&
        (m_values[idx].has_type(ValueType::VALUE_STRING)))
    {
        val = m_values[idx].to_str();
        return true;
    }

    return false;
}

ValueType option_type::get_type() const
{
    return get_type(0);
}

ValueType option_type::get_type(size_t idx) const
{
    return m_values[idx].type();
}

bool option_type::is_type(ValueType type) const
{
    return is_type(0, type);
}

bool option_type::is_type(size_t idx, ValueType type) const
{
    return get_type(idx) == type;
}

long option_type::get_long() const
{
    return get<long>();
}

long option_type::get_long(size_t idx) const
{
    return get<long>(idx);
}

double option_type::get_double() const
{
    return get<double>();
}

double option_type::get_double(size_t idx) const
{
    return get<double>(idx);
}

bool option_type::get_bool() const
{
    return get<bool>();
}

bool option_type::get_bool(size_t idx) const
{
    return get<bool>(idx);
}

const std::string& option_type::get_str() const
{
    return get<const std::string&>();
}

const std::string& option_type::get_str(size_t idx) const
{
    return get<const std::string&>(idx);
}

size_t option_type::size() const
{
    return m_values.size();
}

bool option_type::is_vector() const
{
    return (m_values.size() > 1);
}

const values_vector& option_type::values() const
{
    return m_values;
}

const std::string& option_type::name() const
{
    return m_name;
}

} // configparser
