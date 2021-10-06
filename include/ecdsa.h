#ifndef KEY_H
#define KEY_H

#include <secp256k1.h>
#include <base16.h>
#include <string>
#include <tuple>
#include <vector>
#include <ostream>

namespace encoin {

typedef std::vector<unsigned char> bytes, key_t;

inline std::string key_to_string(key_t key)
{ return std::string(key.begin(), key.end()); }

inline key_t string_to_key(std::string str)
{ return key_t(str.begin(), str.end()); }

class ec_point_exception : public std::runtime_error
{
public:
    ec_point_exception(const std::string &error) noexcept
        : std::runtime_error(error)
    {

    }

    const char *what() const noexcept
    {
        return std::runtime_error::what();
    }
};

class ec_point
{
public:
    ec_point();
    ec_point(const std::string &private_key);
    ~ec_point();

    key_t public_key() const { return _pubkey; }
    key_t private_key() const { return _privkey; }
    key_t public_key_hex() const { return base16_encode(_pubkey); }
    key_t private_key_hex() const { return base16_encode(_privkey); }

    std::tuple<key_t, bool> sign(bytes hash) const;
    static bool verify(bytes msg_hash, const bytes sig, const key_t pubkey);

protected:
    bool verify_key();
    bool create_public_key(bool compressed = false);

protected:
    static constexpr size_t PUBLIC_KEY_SIZE = 65;
    static constexpr size_t SIGNATURE_SIZE  = 72;

private:
    secp256k1_context *_ctx{nullptr};
    key_t _pubkey;
    key_t _privkey;
};

}

inline std::ostream &operator<<(std::ostream &stream, encoin::key_t k)
{
    return stream << std::string(k.begin(), k.end());
}


#endif // KEY_H
