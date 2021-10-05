#include "ecdsa.h"

#include "random.h"
#include <chrono>
#include <random>
#include <tuple>
#include <vector>
#include <sha256.h>
#include <exception>
#include <iostream>

namespace encoin {

ec_point::ec_point()
    : _ctx(secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY))
{
    std::string new_key = generate_random_private_key();
    key_t out(new_key.begin(), new_key.end());
    if (out.size() != 32)
        throw ec_point_exception("size not equal to 32: " + std::to_string(out.size()));

    _privkey = std::move(out);

    if (!verify_key())
        throw ec_point_exception("Unable to create and verify key:  ");

    if (!create_public_key())
        throw ec_point_exception("Unable to create publick key");
}

ec_point::~ec_point()
{
    secp256k1_context_destroy(_ctx);
}

ec_point::ec_point(const std::string& privateKey)
    : _ctx(secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY))
{
    auto priv = encoin::base16_decode(std::vector(privateKey.begin(), privateKey.end()));
    _privkey.assign(privateKey.begin(), privateKey.end());

    if (!verify_key())
        throw ec_point_exception("Unable to create and verify key");

    if (!create_public_key())
        throw ec_point_exception("Unable to create public key");
}

bool ec_point::verify_key()
{
    return secp256k1_ec_seckey_verify(_ctx, _privkey.data());
}

bool ec_point::create_public_key(bool compressed)
{
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(_ctx, &pubkey, _privkey.data()))
        return false;

    size_t outsize = PUBLIC_KEY_SIZE;
    _pubkey.resize(outsize);
    secp256k1_ec_pubkey_serialize(_ctx, _pubkey.data(), &outsize, &pubkey,
        compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
    _pubkey.resize(outsize);
    return true;
}

std::tuple<std::vector<uint8_t>, bool> ec_point::sign(bytes hash) const
{
    secp256k1_ecdsa_signature sig;
    int ret = secp256k1_ecdsa_sign(_ctx, &sig, hash.data(), _privkey.data(),
        secp256k1_nonce_function_rfc6979, nullptr);
    if (ret != 1)
        return std::make_tuple(key_t(), false);

    key_t sigout(SIGNATURE_SIZE);
    size_t sigout_sz = SIGNATURE_SIZE;
    ret = secp256k1_ecdsa_signature_serialize_der(_ctx, &sigout[0], &sigout_sz, &sig);
    if (ret != 1)
        return std::make_tuple(key_t(), false);

    sigout.resize(sigout_sz);
    return std::make_tuple(sigout, true);
}

bool ec_point::verify(bytes msgHash, const bytes sign, const key_t pub_key)
{
    if (pub_key.size() != PUBLIC_KEY_SIZE)
        throw ec_point_exception("Invalid public key size");

    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, pub_key.data(), pub_key.size()))
        return false;

    secp256k1_ecdsa_signature sig;
    if (secp256k1_ecdsa_signature_parse_der(ctx, &sig, sign.data(), sign.size()) == 0)
        return false;

    secp256k1_ecdsa_signature_normalize(ctx, &sig, &sig);
    bool ret = secp256k1_ecdsa_verify(ctx, &sig, msgHash.data(), &pubkey);
    secp256k1_context_destroy(ctx);
    return ret;
}

}
