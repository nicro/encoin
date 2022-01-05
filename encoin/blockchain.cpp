#include "blockchain.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace encoin {

using nlohmann::json;

blockchain::blockchain(bool is_main)
    : _is_main(is_main)
{
    if (_is_main && std::filesystem::exists(CHAIN_FILENAME))
    {
        load(CHAIN_FILENAME);
    }
}

blockchain::~blockchain()
{
    if (_is_main)
    {
        save(CHAIN_FILENAME);
    }
}

blockchain blockchain::operator=(const blockchain &other)
{
    set_blocks(other.blocks());
    set_pool(other.pool());
    save(CHAIN_FILENAME);
    return *this;
}

void blockchain::push(block block)
{
    if (block.hash().empty())
        block.set_hash(block.calc_hash());

    block.set_height(last_block().height() + 1);
    block.set_prev_hash(last_block().calc_hash());

    _blocks.push_back(block);
}

void blockchain::push(const transaction &tx)
{
    _pool.push_back(tx);
}

bool blockchain::contains(const transaction &tx)
{
    for (auto &it : _pool)
    {
        if (tx.to_hash() == it.to_hash())
        {
            return true;
        }
    }
    return false;
}

void blockchain::print()
{
    for (auto &it : _blocks)
    {
        std::cout << "Block " << ": " << it.to_string() << std::endl << std::endl;
    }
}

block blockchain::last_block()
{
    return _blocks.size() ? _blocks.back() : block{};
}

void blockchain::load(const std::string &filename)
{
    std::ifstream str(filename);
    std::stringstream buffer; buffer << str.rdbuf();
    std::string string = buffer.str();
    *this = blockchain::from_string(string);
    str.close();
}

void blockchain::save(const std::string &filename)
{
    std::ofstream str(filename);
    str << to_string() << std::endl;
    str.close();
}

bool blockchain::is_empty()
{
    return _blocks.empty();
}

std::string blockchain::to_string() const
{
    json j;
    std::vector<std::string> list;

    for (auto &it : _blocks)
    {
        list.push_back(it.to_string());
    }
    j["blocks"] = list, list.clear();

    for (auto &it : _pool)
    {
        list.push_back(it.to_string());
    }
    j["pool"] = list;
    return j.dump();
}

blockchain blockchain::from_string(const std::string &string)
{
    blockchain chain;
    json j = json::parse(string);
    for (auto &it : j["blocks"])
    {
        chain.push(block::from_string(it));
    }
    for (auto &it : j["pool"])
    {
        chain.push(transaction::from_string(it));
    }
    return chain;
}

void blockchain::clear_pool()
{
    _pool.clear();
}

amount_t blockchain::get_balance(const pubkey_t &addr)
{
    amount_t balance = 0;
    for (auto &block : _blocks)
    {
        for (auto &tx : block.transactions())
        {
            for (auto &in : tx.inputs())
                if (in.address == addr)
                    balance -= std::min(balance, in.amount);

            for (auto &out : tx.outputs())
                if (out.address == addr)
                    balance += out.amount;
        }
    }
    return balance;
}

blockchain blockchain::random_filled()
{
    blockchain chain;

    block b1 = {
        transaction::create_random(),
        transaction::create_random(),
        transaction::create_random()
    };

    wallet wallet1;
    const auto to = wallet1.create_new_address();

    block b2;

    block b3 = {
        transaction::create(wallet1.create_new_address(), to, 220),
        transaction::create(wallet1.create_new_address(), to, 54)
    };

    chain.push(b1);
    chain.push(b2);
    chain.push(b3);

    return chain;
}

}
