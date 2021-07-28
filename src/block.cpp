#include "block.h"

#include <sha256.h>
#include <sstream>

namespace Encoin {

using std::chrono::system_clock;

Block::Block(int idx)
    : _index(idx)
{
    _timestamp = system_clock::to_time_t(system_clock::now());
}

bool Block::isValid() const
{
    // checks if first digits equal 000000
    return true;
}

void Block::add(const Transaction &tx)
{
    _transactions.push_back(tx);
}

void Block::calcHash()
{
    std::stringstream buffer;
    for (auto &tx : _transactions)
    {
        buffer << tx.receiver() << " ";
        buffer << tx.sender()   << " ";
        buffer << tx.amount()   << " ";
    }
    buffer << _previousHash << std::endl;

    _hash = sha256(buffer.str());
}

std::string Block::hash() const
{
    return _hash;
}

void Block::setPreviousHash(const std::string &hash)
{
    _previousHash = hash;
}

int Block::index() const
{
    return _index;
}

Block Block::genesis()
{
    Block genesis(0);
    genesis.setPreviousHash("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    genesis.add({ "satoshi", "nicro", 50 });
    return genesis;
}

}

