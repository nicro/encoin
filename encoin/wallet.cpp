#include "wallet.h"

#include <sha256.h>
#include <iostream>
#include <ecdsa.h>

namespace encoin {

wallet::wallet()
    : _storage(create_wallet_storage())
{
    _storage.sync_schema();
}

pubkey_t wallet::create_new_address()
{
    ec_point point;
    auto pub_key = point.public_key_hex();
    auto priv_key = point.private_key_hex();
    auto kp = keypair{
        std::string(pub_key.begin(), pub_key.end()),
        std::string(priv_key.begin(), priv_key.end())
    };

    _storage.replace(kp);
    return std::string(pub_key.begin(), pub_key.end());
}

std::list<pubkey_t> wallet::addresses()
{
    std::list<pubkey_t> list;
    for (auto &it : _storage.iterate<keypair>())
        list.push_back(it.public_key);
    return list;
}

pubkey_t wallet::get_active_address()
{
    auto all = addresses();
    return all.empty() ? create_new_address() : all.front();
}

void wallet::remove_all()
{
    _storage.remove_all<keypair>();
}

privkey_t wallet::get_private_key(const std::string &pubkey)
{
    auto kp = _storage.get_pointer<keypair>(pubkey);
    return kp ? kp->private_key : "";
}

transaction wallet::send(const std::string &to, const amount_t &amount)
{
    auto pubkey = get_active_address();
    auto privkey = get_private_key(pubkey);
    ec_point point({privkey.begin(), privkey.end()});

    transaction tx;
    tx._inputs.push_back(input_t{ amount, pubkey });
    tx._outputs.push_back(output_t{ amount, to });

    auto[signature, ok] = point.sign(tx.hash_input(tx._inputs[0]));
    if (!ok) throw std::runtime_error("signature not created");
    tx._inputs[0].signature = signature;
    tx._hash = tx.to_hash();
    try
    {
        tx.validate();
    }
    catch (const transaction_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    return tx;
}   

}
