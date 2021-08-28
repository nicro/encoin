#include "transaction.h"

#include <sha256.h>
#include <sstream>

namespace encoin {

inline std::stringstream &operator<<(std::stringstream &ss, const input_t &in)
{
    return ss << in.index << in.amount << in.address << in.signature << in.transaction, ss;
}

inline std::stringstream &operator<<(std::stringstream &ss, const output_t &out)
{
    return ss << out.index << out.amount << out.address, ss;
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
        throw new transaction_error("hash mismatch");

    auto verifyFunc = [](std::string, std::string, std::string) { return true; };

    for (auto &input : _inputs)
    {
        std::string msg = sha256(input.transaction + std::to_string(input.index) + input.address);
        if (!verifyFunc(input.address, input.signature, msg))
            throw new transaction_error("invalid signature");
    }

    for (auto &input : _inputs)
    {
        if (input.amount < 0)
            throw new transaction_error("zero input");
    }

    double total_input_amount = 0.f;
    for (auto &input : _inputs)
        total_input_amount += input.amount;

    double total_output_amount = 0.f;
    for (auto &output : _outputs)
        total_input_amount += output.amount;

    double spare_amount = (total_input_amount - total_output_amount) - TRANSACTION_FEE;
    if (spare_amount < 0)
        throw new transaction_error("too small fee");
}



}
