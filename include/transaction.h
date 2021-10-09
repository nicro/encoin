#ifndef TRANSCATION_H
#define TRANSCATION_H

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <wallet.h>

namespace encoin {

typedef uint64_t amount_t;

struct input_t {
    amount_t amount;
    std::string address;
    std::string transaction;
    std::string signature;
};

struct output_t {
    amount_t amount;
    std::string address;
};

class transaction_error : public std::runtime_error
{
public:
    transaction_error(const std::string &what)
        : std::runtime_error(what) {}
};

class transaction
{
    using hash_t = std::string;

    static constexpr double TRANSACTION_FEE = 100;

public:
    transaction();

    hash_t to_hash() const;
    void validate() const;

    static transaction create_test(address_t from,
                                   address_t to,
                                   address_t reward,
                                   amount_t amount,
                                   amount_t requested_amount);
    amount_t calc_spare_amount() const;

protected:
    std::vector<input_t> _inputs;
    std::vector<output_t> _outputs;
    hash_t _hash;
};

}


#endif // TRANSCATION_H
