#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>

#include "block.h"

namespace encoin {

class blockchain
{
public:
    blockchain();

    void add(block block);
    void print();

protected:
    std::vector<block> _blocks;

};

}

#endif // BLOCKCHAIN_H
