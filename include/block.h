#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <time.h>

#include "transaction.h"

namespace encoin {

class block
{
public:

    block();

    std::string hash() const;
    bool is_valid() const;

    void calc_hash();
    void set_previous_hash(const std::string &hash);
    void add(const transaction &tx);

    static block genesis();

protected:
    time_t _timestamp;
    std::string _hash;
    std::string _previousHash;
    std::vector<transaction> _transactions;
};

}

#endif // BLOCK_H
