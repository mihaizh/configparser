#include "value_type.h"
#include <cassert> // assert

namespace configparser
{

detail::value_union::value_union()
{
}

detail::value_union::~value_union()
{
}

value_type::value_type(long val)
    : m_type(ValueType::VALUE_LONG)
{
    m_value.l = val;
}

value_type::value_type(double val)
    : m_type(ValueType::VALUE_DOUBLE)
{
    m_value.d = val;
}

value_type::value_type(bool val)
    : m_type(ValueType::VALUE_BOOLEAN)
{
    m_value.b = val;
}

value_type::value_type(std::string&& val)
    : m_type(ValueType::VALUE_STRING)
{
    new (&m_value.str) std::string();
    m_value.str = std::move(val);
}

value_type::value_type(const value_type& other)
    : m_type(other.m_type)
{
    switch (m_type)
    {
        case ValueType::VALUE_LONG:
            m_value.l = other.m_value.l;
            break;
        case ValueType::VALUE_DOUBLE:
            m_value.d = other.m_value.d;
            break;
        case ValueType::VALUE_BOOLEAN:
            m_value.b = other.m_value.b;
            break;
        case ValueType::VALUE_STRING:
            new (&m_value.str) std::string();
            m_value.str = other.m_value.str;
            break;
        default: // should never happen
            break;
    }
}

value_type::value_type(value_type&& other) noexcept
    : m_type(other.m_type)
{
    switch (m_type)
    {
        case ValueType::VALUE_LONG:
            m_value.l = other.m_value.l;
            break;
        case ValueType::VALUE_DOUBLE:
            m_value.d = other.m_value.d;
            break;
        case ValueType::VALUE_BOOLEAN:
            m_value.b = other.m_value.b;
            break;
        case ValueType::VALUE_STRING:
            new (&m_value.str) std::string();
            m_value.str = std::move(other.m_value.str);
            break;
        default: // should never happen
            break;
    }
}

value_type& value_type::operator=(const value_type& other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        switch (m_type)
        {
            case ValueType::VALUE_LONG:
                m_value.l = other.m_value.l;
                break;
            case ValueType::VALUE_DOUBLE:
                m_value.d = other.m_value.d;
                break;
            case ValueType::VALUE_BOOLEAN:
                m_value.b = other.m_value.b;
                break;
            case ValueType::VALUE_STRING:
                new (&m_value.str) std::string();
                m_value.str = other.m_value.str;
                break;
            default: // should never happen
                break;
        }
    }

    return *this;
}

value_type& value_type::operator=(value_type&& other) noexcept
{
    if (this != &other)
    {
        m_type = other.m_type;
        switch (m_type)
        {
            case ValueType::VALUE_LONG:
                m_value.l = other.m_value.l;
                break;
            case ValueType::VALUE_DOUBLE:
                m_value.d = other.m_value.d;
                break;
            case ValueType::VALUE_BOOLEAN:
                m_value.b = other.m_value.b;
                break;
            case ValueType::VALUE_STRING:
                new (&m_value.str) std::string();
                m_value.str = std::move(other.m_value.str);
                break;
            default: // should never happen
                break;
        }
    }

    return *this;
}

value_type::~value_type()
{
    if (m_type == ValueType::VALUE_STRING)
    {
        m_value.str.~basic_string();
    }
}

ValueType value_type::type() const
{
    return m_type;
}

bool value_type::has_type(ValueType type) const
{
    return m_type == type;
}

long value_type::to_long() const
{
    assert(has_type(ValueType::VALUE_LONG));
    return m_value.l;
}

double value_type::to_double() const
{
    assert(has_type(ValueType::VALUE_DOUBLE));
    return m_value.d;
}

bool value_type::to_bool() const
{
    assert(has_type(ValueType::VALUE_BOOLEAN));
    return m_value.b;
}

const std::string& value_type::to_str() const
{
    assert(has_type(ValueType::VALUE_STRING));
    return m_value.str;
}

} // configparser
