#include <configparser.h>
#include <iostream>
#include <cassert>

static const char* cfg_file_text =
"[Section 1]\n"
"; comment\n"
"Option 1 = value 1; option 'Option 1' has value 'value 1'\n"
"oPtion 1 = \\ value 2\\ \\ \\; option 'oPtion 1' has value ' value 2   ', 'oPtion 1' and 'Option 1' are different\n"
"\n"
"[$Section::subsection]; no subsection, only valid identifier of section\n"
"Option 2 = value 1:value 2 : value 3; option 'Option 2' is list of 'value 1', 'value 2' and 'value 3'\n"
"Option 3 = value 1, ${ Section 1#Option 1 }; option 'Option 3' is list of 'value 1' and 'value 1'\n"
"Option 4 = v1, ${ $Section::subsection#Option 3 }, v2; option 'Option 4' is list of 'v1', 'value 1', 'value 1', 'v2'\n"
"Option 5 = v1, v2:v3; option 'Option 5' is list of 'v1' a 'v2:v3'\n"
"\n"
"[Numbers]\n"
"num = -1285\n"
"num_bin = 0b01101001\n"
"num_hex = 0x12ae, 0xAc2B\n"
"num_oct = 01754\n"
"\n"
"float1 = -124.45667356\n"
"float2 = +4.1234565E+45\n"
"float3 = 412.34565e45\n"
"float4 = -1.1245864E-6\n"
"\n"
"[Other]\n"
"bool1 = 1\n"
"bool2 = on\n"
"bool3 = f";

int main(int argc, char* argv[])
{
    configparser::ConfigParser p;
    if (p.parse_text(cfg_file_text))
    {
        for (const configparser::section_type& sct : p.sections())
        {
            std::cout << '[' << sct.name() << ']' << std::endl;

            for (const configparser::option_type& opt : sct.options())
            {
                std::cout << opt.name() << " = ";
                for (const configparser::value_type& val : opt.values())
                {
                    switch (val.type())
                    {
                        case configparser::ValueType::VALUE_LONG:
                            std::cout << val.to_long() << ',';
                            break;
                        case configparser::ValueType::VALUE_DOUBLE:
                            std::cout << val.to_double() << ',';
                            break;
                        case configparser::ValueType::VALUE_BOOLEAN:
                            std::cout << val.to_bool() << ',';
                            break;
                        case configparser::ValueType::VALUE_STRING:
                            std::cout << val.to_str() << ',';
                            break;
                        default:
                            assert(false);
                            break;
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "Failed to parse configuration:" << std::endl;
        std::cerr << "Line " << p.get_error_line() << ", column " << p.get_error_column() << ": ";
        switch (p.error_code())
        {
            case configparser::ErrorCode::UNEXPECTED_CHARACTER:
                std::cerr << "Unexpected character encountered." << std::endl;
                break;
            case configparser::ErrorCode::EXPECTED_CLOSING_BRACKET:
                std::cerr << "Expected closing bracket ']' after section name." << std::endl;
                break;
            case configparser::ErrorCode::EXPECTED_SECTION_FIRST:
                std::cerr << "Expected section before any option." << std::endl;
                break;
            case configparser::ErrorCode::EXPECTING_VALUE_AFTER_IDENTIFIER:
                std::cerr << "Expecting value after option identifier." << std::endl;
                break;
            case configparser::ErrorCode::UNEXPECTED_VALUE:
                std::cerr << "Unexpected value." << std::endl;
                break;
            case configparser::ErrorCode::UNEXPECTED_TOKEN:
                std::cerr << "Unexpected token." << std::endl;
                break;
            default:
                std::cerr << "Unknown error." << std::endl;
                break;
        }
    }

    return 0;
}
