#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <time.h>
#include <sha256.h>
#include "transaction.h"

namespace encoin {

class block
{
public:
    block();
    block(std::initializer_list<transaction> txs);

    template <typename... Args>
    friend auto create_chain_storage();

    void update_time_now();
    void add(const transaction &tx);
    std::string to_string() const;
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

    void load_tx_data();
    void save_tx_data();

protected:
    unsigned _height = 0;
    unsigned _nonce = 0;
    time_t _timestamp;
    std::string _hash;
    std::string _prev_hash;
    std::string _txdata;
    std::vector<transaction> _transactions;
};

}

#endif // BLOCK_H
