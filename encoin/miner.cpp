#include "miner.h"

#include <iostream>
#include <termcolor/termcolor.hpp>
#include <crypto/sha256.h>
#include <settings.h>


namespace encoin {

miner::miner(blockchain &chain, const pubkey_t &reward_address)
    : _chain(chain), _reward_address(reward_address)
{
    settings settings;
    _reward_address = settings.reward_address();
    if (_reward_address.size() == 0)
    {
        std::string addr = wallet().get_active_address();
        _reward_address = addr;
        settings.set_reward_address(addr);
    }
}

void miner::start()
{
    // ToDo: each 10 minutes
    // get all new transactions
    for (;;)
    {
        block new_block;
        for (const auto &tx : _chain.pool())
            new_block.add(tx);
        _chain.clear_pool();

        if (mine_block(new_block))
        {
            std::cout << termcolor::green << "BLOCK MINED" << std::endl;
        }
    }
}

bool miner::mine_block(block &new_block)
{
    block &&prev = _chain.last_block();

    new_block.set_height(prev.height() + 1);
    new_block.set_prev_hash(prev.hash());
    new_block.update_time_now();

    for (unsigned n = 0;; n++)
    {
        new_block.set_nonce(n);
        auto &&can = encoin::sha256(new_block.calc_hash());
        auto &&ncan = std::stoull(can.substr(0, 16), 0, 16);
        uint64_t &&target = get_difficulty();

        if (ncan < target)
        {
            new_block.set_hash(can);
            std::cout << termcolor::yellow
                      << "Block " << new_block.height()
                      << ": nonce: " << n
                      << " target: " << target
                      << std::endl;
            return true;
        }
    }
    return false;
}

uint64_t miner::get_difficulty() const
{
    return 0xfffffffffff;
}

}
