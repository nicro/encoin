#include "ecdsa.h"

namespace encoin {

ec_point::ec_point(const key_t &priv_key)
    : _priv_key(priv_key)
{
    _ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    calc_public_key(true);
}

bool ec_point::verify_key() const {
    return secp256k1_ec_seckey_verify(_ctx, _priv_key.data());
}

bool ec_point::calc_public_key(bool compressed)
{
    secp256k1_pubkey pubkey;
    int ret = secp256k1_ec_pubkey_create(_ctx, &pubkey, _priv_key.data());
    if (ret != 1)
        return false;

    // Serialize public key.
    size_t out_size = PUBLIC_KEY_SIZE;
    _pub_key.resize(out_size);
    secp256k1_ec_pubkey_serialize( _ctx, _pub_key.data(), &out_size, &pubkey,
        compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
    _pub_key.resize(out_size);
    return true;

}

bool ec_point::sign(const key_t &hash, key_t &result) const
{
    secp256k1_ecdsa_signature sig;
    int ret = secp256k1_ecdsa_sign(_ctx, &sig, hash.data(), _priv_key.data(),
                                   secp256k1_nonce_function_rfc6979, nullptr);
    if (ret != 1)
        return false;

    key_t sig_out;
    size_t sig_out_size = 72;
    sig_out.reserve(sig_out_size);
    ret = secp256k1_ecdsa_signature_serialize_der(_ctx, (unsigned char*)sig_out.data(), &sig_out_size, &sig);
    if (ret != 1)
        return false;

    sig_out.resize(sig_out_size);
    result = sig_out;
    return true;
}

static int ecdsa_signature_parse_der_lax(const secp256k1_context *ctx,
                                         secp256k1_ecdsa_signature *sig,
                                         const unsigned char *input,
                                         size_t inputlen) {
  size_t rpos, rlen, spos, slen;
  size_t pos = 0;
  size_t lenbyte;
  unsigned char tmpsig[64] = {0};
  int overflow = 0;

  /* Hack to initialize sig with a correctly-parsed but invalid signature. */
  secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);

  /* Sequence tag byte */
  if (pos == inputlen || input[pos] != 0x30) {
    return 0;
  }
  pos++;

  /* Sequence length bytes */
  if (pos == inputlen) {
    return 0;
  }
  lenbyte = input[pos++];
  if (lenbyte & 0x80) {
    lenbyte -= 0x80;
    if (pos + lenbyte > inputlen) {
      return 0;
    }
    pos += lenbyte;
  }

  /* Integer tag byte for R */
  if (pos == inputlen || input[pos] != 0x02) {
    return 0;
  }
  pos++;

  /* Integer length for R */
  if (pos == inputlen) {
    return 0;
  }
  lenbyte = input[pos++];
  if (lenbyte & 0x80) {
    lenbyte -= 0x80;
    if (pos + lenbyte > inputlen) {
      return 0;
    }
    while (lenbyte > 0 && input[pos] == 0) {
      pos++;
      lenbyte--;
    }
    if (lenbyte >= sizeof(size_t)) {
      return 0;
    }
    rlen = 0;
    while (lenbyte > 0) {
      rlen = (rlen << 8) + input[pos];
      pos++;
      lenbyte--;
    }
  } else {
    rlen = lenbyte;
  }
  if (rlen > inputlen - pos) {
    return 0;
  }
  rpos = pos;
  pos += rlen;

  /* Integer tag byte for S */
  if (pos == inputlen || input[pos] != 0x02) {
    return 0;
  }
  pos++;

  /* Integer length for S */
  if (pos == inputlen) {
    return 0;
  }
  lenbyte = input[pos++];
  if (lenbyte & 0x80) {
    lenbyte -= 0x80;
    if (pos + lenbyte > inputlen) {
      return 0;
    }
    while (lenbyte > 0 && input[pos] == 0) {
      pos++;
      lenbyte--;
    }
    if (lenbyte >= sizeof(size_t)) {
      return 0;
    }
    slen = 0;
    while (lenbyte > 0) {
      slen = (slen << 8) + input[pos];
      pos++;
      lenbyte--;
    }
  } else {
    slen = lenbyte;
  }
  if (slen > inputlen - pos) {
    return 0;
  }
  spos = pos;

  /* Ignore leading zeroes in R */
  while (rlen > 0 && input[rpos] == 0) {
    rlen--;
    rpos++;
  }
  /* Copy R value */
  if (rlen > 32) {
    overflow = 1;
  } else {
    std::memcpy(tmpsig + 32 - rlen, input + rpos, rlen);
  }

  /* Ignore leading zeroes in S */
  while (slen > 0 && input[spos] == 0) {
    slen--;
    spos++;
  }
  /* Copy S value */
  if (slen > 32) {
    overflow = 1;
  } else {
    std::memcpy(tmpsig + 64 - slen, input + spos, slen);
  }

  if (!overflow) {
    overflow = !secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);
  }
  if (overflow) {
    /* Overwrite the result again with a correctly-parsed but invalid
       signature if parsing failed. */
    std::memset(tmpsig, 0, 64);
    secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);
  }
  return 1;
}

pub_key::pub_key(const key_t &data)
    : _data(data)
{
    _ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
}

bool pub_key::verify(const key_t &hash, const key_t &sign) const
{
     secp256k1_pubkey pubkey;
     if (!secp256k1_ec_pubkey_parse(_ctx, &pubkey, _data.data(), _data.size()))
         return false;

     secp256k1_ecdsa_signature sig;
     if (!ecdsa_signature_parse_der_lax(_ctx, &sig, sign.data(), sign.size()))
         return false;

     /* libsecp256k1's ECDSA verification requires lower-S signatures, which have
      * not historically been enforced in Bitcoin, so normalize them first. */
     secp256k1_ecdsa_signature_normalize(_ctx, &sig, &sig);
     return secp256k1_ecdsa_verify(_ctx, &sig, hash.data(), &pubkey);
}



}
