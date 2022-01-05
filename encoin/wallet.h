#ifndef WALLET_H
#define WALLET_H

#include <list>
#include <crypto/ecdsa.h>
#include <transaction.h>

namespace encoin {

struct keypair {
    pubkey_t public_key;
    privkey_t private_key;
};


class wallet {
public:
    wallet();
    ~wallet();

    static constexpr auto WALLET_FILENAME = "wallet.json";

    pubkey_t create_new_address();
    pubkey_t get_active_address();
    std::list<pubkey_t> addresses();

    void save(const std::string &name);
    void load(const std::string &name);
    privkey_t get_private_key(const pubkey_t &pubkey);
    transaction send(const pubkey_t &to, const amount_t &amount);

protected:
    std::vector<keypair> _keypairs;
};

};

#endif // WALLET_H
