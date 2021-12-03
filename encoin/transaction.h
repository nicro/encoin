#ifndef TRANSCATION_H
#define TRANSCATION_H

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <ecdsa.h>
#include <iostream>


namespace encoin {

typedef uint64_t amount_t;

struct input_t {
    amount_t amount;
    std::string address;
    bytes signature;

    std::string serialize() const;
    void deserialize(const std::string &src);
};

struct output_t {
    amount_t amount;
    std::string address;

    std::string serialize() const;
    void deserialize(const std::string &src);
};

class transaction_error : public std::runtime_error
{
public:
    transaction_error(const std::string &what)
        : std::runtime_error(what) {}
};

class transaction;
typedef std::vector<transaction> tx_list;

class transaction
{
public:
    using hash_t = std::string;

    friend class wallet;
    friend class block;

    transaction() = default;
    void validate() const;

    std::vector<input_t> inputs() const { return _inputs; }
    std::vector<output_t> outputs() const { return _outputs; }

    void add_input(const input_t &in) { _inputs.push_back(in); }
    void add_output(const output_t &out) { _outputs.push_back(out); }

    std::string to_string() const;
    hash_t to_hash() const;
    bytes hash_input(const input_t &in) const;

    static transaction create(const std::string &from, const std::string &to, amount_t amount);
    static transaction create_simple(const std::string &address, amount_t amount);
    static transaction create_random();
    static transaction from_string(const std::string &string);

protected:
    std::vector<input_t> _inputs;
    std::vector<output_t> _outputs;
    hash_t _hash;
};

}


#endif // TRANSCATION_H
