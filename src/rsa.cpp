#include "rsa.h"

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <iostream>

namespace Encoin {

RSAPair::~RSAPair()
{
    RSA_free(_keypair);
    free(_private_key);
    free(_public_key);
    BN_free(_bne);
}

RSAPair::RSAPair()
{
    _bio = BIO_new(BIO_s_mem());

    _bne = BN_new();
    int ret = BN_set_word(_bne, PUBLIC_EXPONENT);
    if (ret != 1)
        return;

    _keypair = RSA_new();
    ret = RSA_generate_key_ex(_keypair, KEY_LENGTH, _bne, NULL);
    if (ret != 1)
    {
        std::cout << "key generation error" << std::endl;
        return;
    }

    _private_key = create_rsa(_keypair, PRIVATE_KEY_PEM, (char*)"private_key");
    _public_key  = create_rsa(_keypair, PUBLIC_KEY_PEM, (char*)"public_key");
}

std::string RSAPair::modulus() {
    std::string modulus;

    BIO_reset(_bio);
    _mem = nullptr;

    BN_print(_bio, RSA_get0_n(_keypair));
    BIO_get_mem_ptr(_bio, &_mem);

    if(_mem && _mem->data && _mem->length)
        modulus.assign(_mem->data, _mem->length);

    return modulus;
}

std::string RSAPair::public_key() {
    std::string public_key;

    BIO_reset(_bio);
    _mem = nullptr;

    BN_print(_bio, RSA_get0_e(_keypair));
    BIO_get_mem_ptr(_bio, &_mem);

    if(_mem && _mem->data && _mem->length)
        public_key.assign(_mem->data, _mem->length);

    return public_key;
}

std::string RSAPair::private_key() {
    std::string private_key;

    BIO_reset(_bio);
    _mem = nullptr;

    BN_print(_bio, RSA_get0_d(_keypair));
    BIO_get_mem_ptr(_bio, &_mem);

    if(_mem && _mem->data && _mem->length)
        private_key.assign(_mem->data, _mem->length);

    return private_key;
}

RSAPair::crypt_result_t RSAPair::encrypt(const char *message)
{
    char *encrypt = new char[RSA_size(_public_key)];
    int encrypt_length = RSA_public_encrypt(strlen(message) + 1, (unsigned char*)message, (unsigned char*)encrypt, _public_key, RSA_PKCS1_OAEP_PADDING);
    return std::make_tuple(encrypt, encrypt_length);
}

RSAPair::crypt_result_t RSAPair::decrypt(const char *message, int encrypt_length)
{
    char *decrypt = new char[encrypt_length];
    int decrypt_length = RSA_private_decrypt(encrypt_length, (unsigned char*)message, (unsigned char*)decrypt, _private_key, RSA_PKCS1_OAEP_PADDING);
    return std::make_tuple(decrypt, decrypt_length);
}

RSA *RSAPair::create_rsa(RSA *keypair, int pem_type, char *file_name) {

    RSA   *rsa = NULL;
    FILE  *fp  = NULL;

    if(pem_type == PUBLIC_KEY_PEM) {

        fp = fopen(file_name, "w");
        PEM_write_RSAPublicKey(fp, keypair);
        fclose(fp);

        fp = fopen(file_name, "rb");
        PEM_read_RSAPublicKey(fp, &rsa, NULL, NULL);
        fclose(fp);

    }
    else if(pem_type == PRIVATE_KEY_PEM) {

        fp = fopen(file_name, "w");
        PEM_write_RSAPrivateKey(fp, keypair, NULL, NULL, 0, NULL, NULL);
        fclose(fp);

        fp = fopen(file_name, "rb");
        PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
        fclose(fp);
    }

    return rsa;
}

}
