#include "transaction.h"

#include <sha256.h>
#include <sstream>

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
        throw new transaction_error("hash mismatch");


    // verify signature

    for (auto &input : _inputs)
    {
        if (input.amount < 0)
            throw new transaction_error("zero input");
    }

    if (calc_spare_amount() < TRANSACTION_FEE)
        throw new transaction_error("too small fee");
}

transaction transaction::create_test(address_t from,
                                     address_t to,
                                     address_t reward,
                                     amount_t amount,
                                     amount_t requested_amount)
{
    transaction tx;
    tx._inputs.push_back(input_t{ amount, from });
    tx._outputs.push_back(output_t{ requested_amount, to });
    //tx.validate();
    tx._outputs.push_back(output_t{ tx.calc_spare_amount(), reward });
    tx._hash = tx.to_hash();
    return tx;
}

amount_t transaction::calc_spare_amount() const
{
    double total_input_amount = 0.f;
    for (auto &input : _inputs)
        total_input_amount += input.amount;

    double total_output_amount = 0.f;
    for (auto &output : _outputs)
        total_input_amount += output.amount;

    return (total_input_amount - total_output_amount);
}



}
