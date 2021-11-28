#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>

#include <transaction.h>
#include <block.h>
#include <wallet.h>

namespace encoin {

namespace sql = sqlite_orm;


template <typename... Args>
inline auto create_chain_storage() {
    auto chain = sql::make_table("blockchain",
        sql::make_column("hash", &block::_hash, sql::primary_key()),
        sql::make_column("prevhash", &block::_prev_hash),
        sql::make_column("height", &block::_height),
        sql::make_column("timestamp", &block::_timestamp),
        sql::make_column("txdata", &block::_txdata)
    );
    return sql::make_storage("chain.sqlite", chain);
}

class blockchain
{
public:
    using storage_t = decltype(create_chain_storage());
    
    blockchain();

    void push(block block);
    void print();
    void remove_all();
    block last_block();

    amount_t get_balance(const pubkey_t &addr);

protected:
    storage_t _storage;
};

}

#endif // BLOCKCHAIN_H
