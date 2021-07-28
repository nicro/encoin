#include "blockchain.h"
#include <iostream>

namespace Encoin {

Blockchain::Blockchain()
{
    _blocks.push_back(Block::genesis());
    _blocks.front().calcHash();
}

void Blockchain::add(Block block)
{
    block.setPreviousHash(_blocks.back().hash());
    block.calcHash();
    _blocks.push_back(block);
}

void Blockchain::print()
{
    for (Block &block : _blocks)
    {
        std::cout << "Block " << block.index() << ": " << block.hash() << std::endl;
    }
}



}
