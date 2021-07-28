#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <time.h>

#include "transaction.h"

namespace Encoin {

class Block
{
public:

    Block(int idx);

    int index() const;
    std::string hash() const;
    bool isValid() const;

    void calcHash();
    void setPreviousHash(const std::string &hash);
    void add(const Transaction &tx);

    static Block genesis();

protected:
    int _index;
    time_t _timestamp;
    std::string _hash;
    std::string _previousHash;
    std::vector<Transaction> _transactions;
};

}

#endif // BLOCK_H
