#include <iostream>
#include <wallet.h>

int main()
{
    encoin::wallet db;
    db.create_new_address();
    db.create_new_address();
    db.create_new_address();

    for (auto &as : db.addresses())
        std::cout << as << std::endl;
    return 0;
}
