#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <transaction.h>
#include <block.h>
#include <wallet.h>

namespace encoin {

class blockchain
{
public:
    blockchain(bool is_main = false);
    ~blockchain();

    blockchain operator=(const blockchain &other);

    static constexpr auto CHAIN_FILENAME = "chain.json";

    void push(block block);
    void push(const transaction &tx);
    bool contains(const transaction &tx);
    bool is_empty();
    void print();
    void clear_pool();
    block last_block();
    void load(const std::string &filename);
    void save(const std::string &filename);
    std::string to_string() const;
    amount_t get_balance(const pubkey_t &addr);
    static blockchain random_filled();
    static blockchain from_string(const std::string &string);

    block_list blocks() const { return _blocks; };
    void set_blocks(const block_list &bl) { _blocks = bl; }

    tx_list pool() const { return _pool; };
    void set_pool(const tx_list &txs) { _pool = txs; }

protected:
    block_list _blocks;
    tx_list _pool;
    bool _is_main;
};

}

#endif // BLOCKCHAIN_H
