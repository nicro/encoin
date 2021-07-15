#include <iostream>
#include <sha256.h>

using namespace Encoin;

int main()
{
    std::cout << sha256("my_super_secret_data") << std::endl;
    return 0;
}
