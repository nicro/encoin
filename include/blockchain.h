#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>

#include <transaction.h>
#include <block.h>
#include <wallet.h>

namespace encoin {

class blockchain
{
public:
    blockchain();

    void push(block block);
    void print();
    block last_block() const { return _blocks.back(); }

    amount_t get_balance_for_address(const address_t &addr);

protected:
    std::vector<block> _blocks;

};

}

#endif // BLOCKCHAIN_H
