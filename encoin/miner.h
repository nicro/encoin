#ifndef MINER_H
#define MINER_H

#include <blockchain.h>
#include <wallet.h>
#include <block.h>

namespace encoin {

class miner {
public:
    miner(blockchain &chain, const pubkey_t &reward_address);

    void start();
    bool mine_block(block &new_block);
    uint64_t get_difficulty() const;

protected:
    blockchain &_chain;
    pubkey_t _reward_address;
};

}

#endif // MINER_H
