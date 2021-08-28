#include "ecdsa.h"
#include <openssl/obj_mac.h>
#include <ostream>

namespace encoin {

    inline std::ostream &operator<<(std::ostream &stream, key_t data)
    {
        for (auto ch : data)
            stream << ch;
        return stream;
    }

    ec_point::ec_point()
    {
        _key = EC_KEY_new();
        _group = EC_GROUP_new_by_curve_name(NID_secp192k1);

        EC_KEY_set_group(_key, _group);
    }

    bool ec_point::generate()
    {
        return 1 == EC_KEY_generate_key(_key);
    }

    bool ec_point::verify(key_t hash, ECDSA_SIG *signature)
    {
        return 1 == ECDSA_do_verify(hash.c_str(), hash.size(), signature, _key);
    }

    ECDSA_SIG *ec_point::sign(key_t hash)
    {
        return ECDSA_do_sign(hash.c_str(), hash.size(), _key);
    }

    key_t ec_point::public_key() const
    {
        int size = i2o_ECPublicKey(_key, NULL);
        if (!size)
            return {};

        key_t pub_key(size, 0);
        unsigned char* begin = &pub_key[0];
        if (i2o_ECPublicKey(_key, &begin) != size)
            return {};

        return pub_key;
    }

    key_t ec_point::private_key() const
    {
        int size = i2d_ECPrivateKey(_key, NULL);
        if (!size)
            return {};

        key_t priv_key(size, 0);
        unsigned char* pbegin = &priv_key[0];
        if (i2d_ECPrivateKey(_key, &pbegin) != size)
            return {};

        return priv_key;
    }

    void ec_point::set_public_key(key_t key)
    {
        const unsigned char* begin = &key[0];
        o2i_ECPublicKey(&_key, &begin, key.size());
    }

    void ec_point::set_private_key(key_t key)
    {
        const unsigned char* begin = &key[0];
        o2i_ECPublicKey(&_key, &begin, key.size());
    }

    ec_point::~ec_point() {
        EC_GROUP_free(_group);
        EC_KEY_free(_key);
    }

}
