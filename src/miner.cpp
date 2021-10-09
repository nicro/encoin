#include "miner.h"

#include <sha256.h>

namespace encoin {

miner::miner(const address_t &reward_address)
    : _reward_address(reward_address)
{
}

void miner::start()
{
    for (;;)
    {
        block new_block;
        if (!mine_block(new_block))
            continue;
        std::cout << "BLOCK MINED" << std::endl;
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
