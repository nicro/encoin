#ifndef TRANSCATION_H
#define TRANSCATION_H

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace encoin {

struct input_t {
    size_t index;
    unsigned amount;
    std::string address;
    std::string transaction;
    std::string signature;
};

struct output_t {
    size_t index;
    unsigned amount;
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

protected:
    std::vector<input_t> _inputs;
    std::vector<output_t> _outputs;
    hash_t _hash;
};

}


#endif // TRANSCATION_H
