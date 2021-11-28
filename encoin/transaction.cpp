#include "transaction.h"

#include <sha256.h>
#include <sstream>
#include <ecdsa.h>
#include <nlohmann/json.hpp>

namespace encoin {

using nlohmann::json;

bytes transaction::hash_input(const input_t &in) const
{
    auto hash = in.serialize();
    return { hash.begin(), hash.end() };
}

transaction::hash_t transaction::to_hash() const
{
    return sha256(to_string());
}

std::string transaction::to_string() const
{
    std::vector<json> inputs;
    for (auto &in : _inputs)
    {
        inputs.push_back(json::parse(in.serialize()));
    }

    std::vector<json> outputs;
    for (auto &out : _outputs)
    {
        outputs.push_back(json::parse(out.serialize()));
    }

    return json{{"inputs", inputs}, {"outputs", outputs}}.dump();
}

transaction transaction::from_string(const std::string &string)
{
    transaction tx;
    json j = json::parse(string);

    for (const auto &v : j["inputs"])
    {
        input_t in; in.deserialize(v.dump());
        tx.add_input(in);
    }
    for (const auto &v : j["outputs"])
    {
        output_t out; out.deserialize(v.dump());
        tx.add_output(out);
    }
    return tx;
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

transaction transaction::create(const std::string &from, const std::string &to, amount_t amount)
{
    transaction tx;
    tx._inputs.push_back(input_t{ amount, from, {}});
    tx._outputs.push_back(output_t{amount, to});
    return tx;
}

transaction transaction::create_simple(const std::string &address, amount_t amount)
{
    transaction tx;
    tx._inputs.push_back(input_t{ amount, address, {}});
    tx._outputs.push_back(output_t{amount, address});
    return tx;
}

transaction transaction::create_random() 
{
    key_t pk = ec_point().public_key_hex();
    amount_t amount = rand() % 5000 + 100;
    return transaction::create_simple(std::string(pk.begin(), pk.end()), amount);
}

std::string input_t::serialize() const
{
    return json{{"amount", amount}, {"address", address}}.dump();
}

void input_t::deserialize(const std::string &src)
{
    json j = json::parse(src);
    amount = j["amount"];
    address = j["address"];
}

std::string output_t::serialize() const
{
    return json{{"amount", amount}, {"address", address}}.dump();
}

void output_t::deserialize(const std::string &src)
{
    json j = json::parse(src);
    amount = j["amount"];
    address = j["address"];
}

}
