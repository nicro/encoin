#ifndef RSA_H
#define RSA_H

#include <openssl/rsa.h>
#include <string>

namespace Encoin {

class RSAPair {

public:
    using crypt_result_t = std::tuple<const char*, int>;

    ~RSAPair();
    RSAPair();

    std::string modulus();
    std::string public_key();
    std::string private_key();

    crypt_result_t encrypt(const char *message);
    crypt_result_t decrypt(const char *message, int encrypt_length);

    RSA * create_rsa(RSA * keypair, int pem_type, char *file_name);

    static constexpr unsigned KEY_LENGTH      = 2048;
    static constexpr unsigned PUBLIC_EXPONENT = 59;
    static constexpr unsigned PUBLIC_KEY_PEM  = 1;
    static constexpr unsigned PRIVATE_KEY_PEM = 0;

private:
    RSA *_private_key{nullptr};
    RSA *_public_key{nullptr};
    RSA *_keypair{nullptr};
    BIGNUM *_bne{nullptr};
    BIO *_bio{nullptr};
    BUF_MEM *_mem{nullptr};
};

}

#endif // RSA_H
