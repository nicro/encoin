#include "wallet.h"

#include <sha256.h>

namespace encoin {

wallet::wallet()
{
}

wallet::~wallet()
{
}

address_t wallet::create_new_address()
{
    auto pair = std::make_shared<ec_point>();
    _keypairs.push_back(pair);
    key_t pubkey = pair->public_key_hex();
    std::string hashed = sha256({ pubkey.begin(), pubkey.end() });
    return { hashed.begin(), hashed.end() };
}

std::list<address_t> wallet::addresses() const
{
    std::list<address_t> list;
    for (auto &it : _keypairs)
        list.push_back(it->public_key_hex());
    return list;
}

void wallet::save()
{

}

}
