#include "blockchain.h"
#include <iostream>

namespace encoin {

blockchain::blockchain()
{
    _blocks.push_back(block::genesis());
    _blocks.front().calc_hash();
}

void blockchain::add(block block)
{
    block.set_prev_hash(_blocks.back().hash());
    block.calc_hash();
    _blocks.push_back(block);
}

void blockchain::print()
{
    for (block &block : _blocks)
    {
        std::cout << "Block " << ": " << block.hash() << std::endl;
    }
}

amount_t blockchain::get_balance_for_address(const address_t &addr)
{
    return 0;
}



}
