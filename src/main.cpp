#include <iostream>
#include <wallet.h>

int main()
{
    encoin::wallet wallet;
    wallet.create_new_address();
    wallet.create_new_address();
    wallet.create_new_address();
    for (auto &add : wallet.addresses())
        std::cout << add << std::endl;
    return 0;
}
