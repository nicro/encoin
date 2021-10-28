#ifndef BASE16_H
#define BASE16_H

#include <string>
#include <vector>

namespace encoin {

int hex_value(char digit);

template <typename type = unsigned char>
static std::vector<type> base16_encode(const std::vector<unsigned char> str, bool capital = false)
{
    std::vector<type> hexstr;
    hexstr.resize(str.size() * 2);
    const size_t a = capital ? 'A' - 1 : 'a' - 1;

    for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF)
    {
        hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
        hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
    }
    return hexstr;
}

template <typename type = unsigned char>
static std::vector<type> base16_decode(const std::vector<unsigned char> hexstr)
{
    std::vector<type> str;
    str.resize((hexstr.size() + 1) / 2);

    for (size_t i = 0, j = 0; i < str.size(); i++, j++)
    {
        str[i] = (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
        str[i] |= (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) & 0xF;
    }
    return str;
}

}

#endif // BASE16_H
