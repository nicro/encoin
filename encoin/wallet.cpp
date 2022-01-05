#include "wallet.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <crypto/sha256.h>
#include <crypto/ecdsa.h>
#include <nlohmann/json.hpp>

namespace encoin {

using nlohmann::json;

wallet::wallet()
{
    if (std::filesystem::exists(WALLET_FILENAME))
    {
        load(WALLET_FILENAME);
    }
}

wallet::~wallet()
{
    save(WALLET_FILENAME);
}

pubkey_t wallet::create_new_address()
{
    ec_point point;
    auto pub_key = point.public_key_hex();
    auto priv_key = point.private_key_hex();
    auto kp = keypair{
        pubkey_t(pub_key.begin(), pub_key.end()),
        privkey_t(priv_key.begin(), priv_key.end())
    };

    _keypairs.push_back(kp);
    return pubkey_t(pub_key.begin(), pub_key.end());
}

std::list<pubkey_t> wallet::addresses()
{
    std::list<pubkey_t> list;
    for (const auto &it : _keypairs)
    {
        list.push_back(it.public_key);
    }
    return list;
}

void wallet::save(const std::string &name)
{
    std::ofstream out(WALLET_FILENAME);

    std::vector<json> list;
    for (auto &it : _keypairs)
    {
        json pp = {
            { "private", it.private_key },
            { "public", it.public_key }
        };
        list.push_back(pp);
    }
    out << json(list).dump();
    out.close();
}

void wallet::load(const std::string &name)
{
    std::ifstream t(WALLET_FILENAME);
    std::stringstream buffer; buffer << t.rdbuf();
    std::string string = buffer.str();

    try
    {
        json j = json::parse(string);
        for (auto &it : j)
        {
            if (it.contains("private") && it.contains("public"))
            {
                _keypairs.push_back(keypair{it["private"], it["public"]});
            }
        }
    }
    catch (...) {}
}

pubkey_t wallet::get_active_address()
{
    return _keypairs.empty() ? create_new_address() : _keypairs.front().public_key;
}

privkey_t wallet::get_private_key(const std::string &pubkey)
{
    for (const auto &it : _keypairs)
    {
        if (it.public_key == pubkey)
        {
            return it.private_key;
        }
    }
    return {};
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
