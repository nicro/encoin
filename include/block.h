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

    void update_time_now();
    void add(const transaction &tx);
    std::string calc_hash() const;
    static block genesis();

    void set_height(const unsigned &height) { _height = height; }
    unsigned height() const { return _height; }

    void set_nonce(const unsigned &nonce) { _nonce = nonce; }
    unsigned nonce() const { return _nonce; }

    std::string hash() const { return _hash; }
    void set_hash(const std::string &hash) { _hash = hash; }

    std::string prev_hash() const { return _prev_hash; }
    void set_prev_hash(const std::string &prev) { _prev_hash = prev; }

    std::vector<transaction> transactions() const { return _transactions; }

protected:
    unsigned _height;
    unsigned _nonce;
    time_t _timestamp;
    std::string _hash;
    std::string _prev_hash;
    std::vector<transaction> _transactions;
};

}

#endif // BLOCK_H
