#include "transaction.h"

#include <sha256.h>
#include <sstream>

namespace Encoin {

transaction::transaction(std::string from, std::string to, double value)
{
}

transaction::hash_t transaction::to_hash() const
{
    std::stringstream buffer;

    for (auto &tx : _inputs)
    {
        buffer << tx.address;
        buffer << tx.amount;
        buffer << tx.signature;
        buffer << tx.transaction;
    }
    for (auto &tx : _outputs)
    {
        buffer << tx.address;
        buffer << tx.amount;
    }

    return sha256(buffer.str());
}

bool transaction::is_valid() const
{
    if (_hash != this->to_hash())
        return false;



    return true;
}



}
