#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <time.h>
#include <crypto/sha256.h>
#include <transaction.h>

namespace encoin {

class block;
typedef std::vector<block> block_list;

class block
{
public:
    block();
    block(std::initializer_list<transaction> txs);

    void update_time_now();
    void add(const transaction &tx);
    std::string to_string() const;
    std::string calc_hash() const;
    static block genesis();
    static block from_string(const std::string &string);

    void set_height(const unsigned &height) { _height = height; }
    unsigned height() const { return _height; }

    void set_nonce(const unsigned &nonce) { _nonce = nonce; }
    unsigned nonce() const { return _nonce; }

    std::string hash() const { return _hash; }
    void set_hash(const std::string &hash) { _hash = hash; }

    std::string prev_hash() const { return _prev_hash; }
    void set_prev_hash(const std::string &prev) { _prev_hash = prev; }

    time_t timestamp() const { return _timestamp; }
    void set_timestamp(const time_t &time) { _timestamp = time; }

    tx_list transactions() const { return _transactions; }
    void set_transactions(const tx_list &txs) { _transactions = txs; }

public:
    unsigned _height = 0;
    unsigned _nonce = 0;
    time_t _timestamp;
    std::string _hash;
    std::string _prev_hash;
    tx_list _transactions;
};

}

#endif // BLOCK_H
