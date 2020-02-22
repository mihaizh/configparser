#ifndef CP_VALUE_TYPE_H
#define CP_VALUE_TYPE_H

#include <string>
#include <vector>

namespace configparser
{
namespace detail
{
    union value_union
    {
        long l;
        double d;
        bool b;
        std::string str;

        value_union();
        ~value_union();
    }; // value_union
}

    enum class ValueType
    {
        VALUE_LONG,
        VALUE_DOUBLE,
        VALUE_BOOLEAN,
        VALUE_STRING,

        VALUE_NUM
    }; // ValueType

    struct value_type
    {
        value_type() = default;
        value_type(long val);
        value_type(double val);
        value_type(bool val);
        value_type(std::string&& val);

        value_type(const value_type&);
        value_type(value_type&&) noexcept;
        value_type& operator=(const value_type&);
        value_type& operator=(value_type&&) noexcept;
        ~value_type();

        ValueType type() const;
        bool has_type(ValueType type) const;

        long to_long() const;
        double to_double() const;
        bool to_bool() const;
        const std::string& to_str() const;

    private:
        detail::value_union m_value;
        ValueType m_type = ValueType::VALUE_NUM;
    }; // value_type

    using values_vector = std::vector<value_type>;

} // configparser

#endif // CP_VALUE_TYPE_H
