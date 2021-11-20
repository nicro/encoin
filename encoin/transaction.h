#ifndef TRANSCATION_H
#define TRANSCATION_H

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <ecdsa.h>

namespace encoin {

typedef uint64_t amount_t;

struct input_t {
    amount_t amount;
    std::string address;
    bytes signature;
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
    friend class wallet;
    friend class block;

    transaction();

    hash_t to_hash() const;
    void validate() const;
    bytes hash_input(const input_t &in) const;

    amount_t calc_spare_amount() const;

    std::vector<input_t> inputs() const { return _inputs; }
    std::vector<output_t> outputs() const { return _outputs; }
protected:
    std::vector<input_t> _inputs;
    std::vector<output_t> _outputs;
    hash_t _hash;
};

}


#endif // TRANSCATION_H
