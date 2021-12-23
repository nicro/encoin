#include "blockchain.h"
#include <iostream>

namespace encoin {

blockchain::blockchain()
    : _storage(create_chain_storage())
{
    _storage.sync_schema();
}

void blockchain::remove_all()
{
    _storage.remove_all<block>();
    _storage.replace(block::genesis());
}

std::vector<block> blockchain::get_all()
{
    return _storage.get_all<block>();
}

void blockchain::push(block block)
{
    if (block.hash().empty())
        block.set_hash(block.calc_hash());

    block.set_height(last_block().height() + 1);
    block.set_prev_hash(last_block().calc_hash());
    block.save_tx_data();

    _storage.replace(block);
}

void blockchain::push(const transaction &tx)
{
    _pool.push_back(tx);
}

block blockchain::last_block()
{
    // should be optimized not to load all records at once
    return _storage.get_all<block>().back();
}

tx_list blockchain::pool() const
{
    return _pool;
}

void blockchain::clear_pool()
{
    _pool.clear();
}

void blockchain::print()
{
    for (auto it : _storage.iterate<block>())
    {
        it.load_tx_data();
        std::cout << "Block " << ": " << it.to_string() << std::endl << std::endl;
    }
}

amount_t blockchain::get_balance(const pubkey_t &addr)
{
    amount_t balance = 0;
    for (auto block : _storage.iterate<block>())
    {
        block.load_tx_data();
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
