#ifndef CP_OPTION_TYPE_H
#define CP_OPTION_TYPE_H

#include "value_type.h"

namespace configparser
{

    class option_type
    {
    public:
        option_type(std::string&& name) noexcept;
        option_type(const option_type&) = default;
        option_type(option_type&&) noexcept = default;
        option_type& operator=(const option_type&) = default;
        option_type& operator=(option_type&&) noexcept = default;
        ~option_type() = default;

        template <typename ValueType>
        ValueType get() const;
        template <typename ValueType>
        ValueType get(size_t idx) const;
        template <typename ValueType>
        bool get(size_t idx, ValueType& val) const;

        long get_long() const;
        long get_long(size_t idx) const;
        double get_double() const;
        double get_double(size_t idx) const;
        bool get_bool() const;
        bool get_bool(size_t idx) const;
        const std::string& get_str() const;
        const std::string& get_str(size_t idx) const;

        ValueType get_type() const;
        ValueType get_type(size_t idx) const;

        bool is_type(ValueType type) const;
        bool is_type(size_t idx, ValueType type) const;

        size_t size() const;
        bool is_vector() const;

        const values_vector& values() const;
        const std::string& name() const;

    private:
        std::string m_name;
        values_vector m_values;

        friend class ConfigParser;
    }; // option_type

    template <typename ValueType>
    ValueType option_type::get() const
    {
        return get<ValueType>(0);
    }

    using option_vector = std::vector<option_type>;

} // configparser

#endif // CP_OPTION_TYPE_H
