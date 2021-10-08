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
    auto pub_key = point.public_key_hex();
    auto priv_key = point.private_key_hex();
    auto kp = keypair{
        std::string(pub_key.begin(), pub_key.end()),
        std::string(priv_key.begin(), priv_key.end())
    };

    _storage.replace(kp);
    return pubkey_to_address(pub_key);
}

std::list<address_t> wallet::addresses()
{
    std::list<address_t> list;
    for (auto &it : _storage.iterate<keypair>())
        list.push_back(it.public_key);
    return list;
}

void wallet::remove_all()
{
    _storage.remove_all<keypair>();
}

std::string wallet::get_private_key(const std::string &pubkey)
{
    auto kp = _storage.get_pointer<keypair>(pubkey);
    return kp ? kp->private_key : "";
}

address_t wallet::pubkey_to_address(const key_t &pub)
{
    std::string hashed = sha256({ pub.begin(), pub.end() });
    return { hashed.begin(), hashed.end() };
}

}
