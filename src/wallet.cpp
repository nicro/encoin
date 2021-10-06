#include "wallet.h"

#include <sha256.h>

namespace encoin {

wallet::wallet()
    : _storage(make_storage_query())
{
    _storage.sync_schema();
}

wallet::~wallet()
{
}

address_t wallet::create_new_address()
{
    ec_point point;
    auto priv_key = point.private_key_hex();
    auto pub_key = point.public_key_hex();
    auto kp = keypair{
        std::string(priv_key.begin(), priv_key.end()),
        std::string(pub_key.begin(), pub_key.end())
    };

    _storage.insert(kp);
    return pubkey_to_address(pub_key);
}

std::list<address_t> wallet::addresses()
{
    std::list<address_t> list;
    for (auto &it : _storage.iterate<keypair>())
        list.push_back(it.public_key);
    return list;
}

address_t wallet::pubkey_to_address(const key_t &pub)
{
    std::string hashed = sha256({ pub.begin(), pub.end() });
    return { hashed.begin(), hashed.end() };
}

}
