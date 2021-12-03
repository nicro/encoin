#ifndef MINER_H
#define MINER_H

#include <blockchain.h>
#include <wallet.h>
#include <block.h>
#include <atomic>

namespace encoin {

class miner {
public:
    miner(const blockchain &chain, const pubkey_t &reward_address);

    void start();
    bool mine_block(block &new_block);

    uint64_t get_difficulty() const;
    void add_transaction(const transaction &tx);

protected:
    blockchain _chain;
    tx_list _pool;
    pubkey_t _reward_address;
};

}

#endif // MINER_H
