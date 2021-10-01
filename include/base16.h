#ifndef BASE16_H
#define BASE16_H

#include <string>

namespace encoin {

int hex_value(char digit);

template <typename type = char>
static std::basic_string<type> base16_encode(const std::basic_string<type> &input)
{
    static constexpr char hex_digits[] = "0123456789ABCDEF";

    std::basic_string<type> output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

template <typename type = char>
static std::basic_string<type> base16_decode(const std::basic_string<type> &input)
{
    const auto len = input.length();
    if (len & 1)
        return "";

    std::basic_string<type> output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end();) {
        int hi = hex_value(*it++);
        int lo = hex_value(*it++);
        if (hi == -1 || lo == -1)
            return "";
        output.push_back(hi << 4 | lo);
    }
    return output;
}

}

#endif // BASE16_H
