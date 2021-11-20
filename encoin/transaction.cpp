#include "transaction.h"

#include <sha256.h>
#include <sstream>
#include <ecdsa.h>

namespace encoin {

inline std::stringstream &operator<<(std::stringstream &ss, const input_t &in)
{
    return ss << in.amount << in.address, ss;
}

inline std::stringstream &operator<<(std::stringstream &ss, const output_t &out)
{
    return ss << out.amount << out.address, ss;
}

transaction::transaction()
{
}

transaction::hash_t transaction::to_hash() const
{
    std::stringstream buffer;

    for (auto &in : _inputs)
        buffer << in;

    for (auto &out : _outputs)
        buffer << out;

    return sha256(buffer.str());
}

void transaction::validate() const
{
    if (_hash != this->to_hash())
        throw transaction_error("hash mismatch");

    if (_inputs.empty()) 
        throw transaction_error("no inputs provided");

    for (auto &input : _inputs)
        if (input.amount < 0)
            throw transaction_error("zero input");


    for (auto &input : _inputs)
    {
        auto pk = key_t{ input.address.begin(), input.address.end() };
        if (!ec_point::verify(hash_input(input), input.signature, base16_decode(pk)))
            throw transaction_error("input not verified");
    }
}

bytes transaction::hash_input(const input_t &in) const
{
    auto hash = (std::stringstream() << in).str();
    return { hash.begin(), hash.end() };
}

amount_t transaction::calc_spare_amount() const
{
    amount_t total_input_amount = 0;
    for (auto &input : _inputs)
        total_input_amount += input.amount;

    amount_t total_output_amount = 0;
    for (auto &output : _outputs)
        total_input_amount += output.amount;

    return (total_input_amount - total_output_amount);
}



}
