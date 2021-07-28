#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>

#include "block.h"

namespace Encoin {

class Blockchain
{
public:
    Blockchain();

    void add(Block block);
    void print();

protected:
    std::vector<Block> _blocks;

};

}

#endif // BLOCKCHAIN_H
