#include "block.h"

namespace encoin {

using std::chrono::system_clock;

block::block()
{
    _timestamp = system_clock::to_time_t(system_clock::now());
}

bool block::is_valid() const
{
    // checks if first digits equal 000000
    return true;
}

void block::add(const transaction &tx)
{
    _transactions.push_back(tx);
}

void block::calc_hash()
{
}

std::string block::hash() const
{
    return _hash;
}

void block::set_previous_hash(const std::string &hash)
{
    _previousHash = hash;
}

block block::genesis()
{
    block genesis;
    //genesis.set_previous_hash("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    //genesis.add({ "satoshi", "nicro", 50 });
    return genesis;
}

}

