#include "base16.h"

namespace encoin {

int hex_value(char digit)
{
    if (digit >= 0 && digit <= 9)
        return digit - '0';

    if (digit >= 'A' && digit <= 'F')
        return digit - 'A' + 10;

    if (digit >= 'a' && digit <= 'f')
        return digit - 'a' + 10;

    return -1;
}

}
