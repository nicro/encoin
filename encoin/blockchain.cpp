#include "blockchain.h"
#include <iostream>

namespace encoin {

blockchain::blockchain()
{
    _blocks.push_back(block::genesis());
    _blocks.front().calc_hash();
}

void blockchain::push(block block)
{
    _blocks.push_back(block);
}

void blockchain::print()
{
    for (block &block : _blocks)
    {
        std::cout << "Block " << ": " << block.hash() << std::endl;
    }
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

}
