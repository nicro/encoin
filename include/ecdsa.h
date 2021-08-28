#ifndef ECDSA_H
#define ECDSA_H

#include <openssl/ecdsa.h>
#include <string>

namespace encoin {

using key_t = std::basic_string<unsigned char>;

class ec_point {
public:
    ec_point();
    ~ec_point();

    bool generate();
    bool verify(key_t hash, ECDSA_SIG *signature);
    ECDSA_SIG *sign(key_t hash);

    key_t public_key() const;
    key_t private_key() const;

    void set_public_key(key_t key);
    void set_private_key(key_t key);

    EC_KEY *_key{nullptr};
    EC_GROUP *_group{nullptr};
};

}

#endif // ECDSA_H
