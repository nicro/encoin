#ifndef KEY_H
#define KEY_H

#include <string>
#include <iostream>
#include <secp256k1.h>
#include <base16.h>

namespace encoin {

typedef std::basic_string<unsigned char> key_t;
#define KEY(_k) reinterpret_cast<const unsigned char*>(_k)

inline std::ostream &operator<<(std::ostream &stream, key_t key)
{
    for (auto ch : key)
        stream << ch;
    return stream;
}

class pub_key {

    pub_key(const key_t &data);
    bool verify(const key_t &hash, const key_t &sign) const;

private:
    key_t _data;
    secp256k1_context_struct *_ctx{nullptr};
};

class ec_point {

public:
    ec_point(const key_t &priv_key);

    bool verify_key() const;
    bool calc_public_key(bool compressed = false);
    bool sign(const key_t &hash, key_t &result) const;

    key_t private_key() const { return _priv_key; }
    key_t public_key() const { return _pub_key; }

    key_t private_key_hex() const { return base16_encode(_priv_key); }
    key_t public_key_hex()  const { return base16_encode(_pub_key);  }

protected:
    static constexpr unsigned PRIVATE_KEY_STORE_SIZE = 32;
    static constexpr unsigned PRIVATE_KEY_SIZE = 279;
    static constexpr unsigned PUBLIC_KEY_SIZE = 65;
    static constexpr unsigned SIGNATURE_SIZE = 72;

private:
    secp256k1_context_struct *_ctx{nullptr};
    key_t _priv_key;
    key_t _pub_key;
};

}

#endif // KEY_H
