#include <blockchain.h>
#include <ecdsa.h>
#include <string>
#include <iostream>

int main()
{
    //encoin::ec_point p{"f7c32876271e88dbb576d575170de7162aed93a398deec0f7fdb330bc3f49956"};
    encoin::ec_point p{};
    std::cout << "Private key: " << p.private_key_hex() << std::endl;
    std::cout << "Public key: " << p.public_key_hex() << std::endl;

    std::string mystr = "MySuperRandomDataMySuperRandomDataMySuperRandomDataMySuperRandomDataMySuperRandomDataMySuperRandomDataMySuperRandomDataMySuperWW";
    encoin::key_t x(mystr.begin(), mystr.end());

    auto[res, success] = p.sign(x);
    if (success) {
        std::cout << "signature success\n";

        if (encoin::ec_point::verify(x, res, p.public_key()))
            std::cout << "Verified!!!\n";
        else
            std::cout << "Not Verified!!!\n";
    }
    return 0;
}
