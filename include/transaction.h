#ifndef TRANSCATION_H
#define TRANSCATION_H

#include <string>
#include <vector>

namespace Encoin {

struct input_t {
    std::string address;
    unsigned int amount;
    std::string transaction;
    std::string signature;
};

struct output_t {
    std::string address;
    unsigned int amount;
};

class transaction
{
    using hash_t = std::string;

public:
    transaction(std::string from, std::string to, double value);

    hash_t to_hash() const;
    bool is_valid() const;

protected:
    std::vector<input_t> _inputs;
    std::vector<output_t> _outputs;
    hash_t _hash;
};

}


#endif // TRANSCATION_H
