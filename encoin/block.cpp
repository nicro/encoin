#include "block.h"
#include <nlohmann/json.hpp>

namespace encoin {

using std::chrono::system_clock;
using nlohmann::json;

block::block()
{
    update_time_now();
}

block::block(std::initializer_list<transaction> txs)
    : _transactions(txs)
{
    update_time_now();
}

void block::add(const transaction &tx)
{
    _transactions.push_back(tx);
}

std::string block::to_string() const 
{
    json j;
    j["nonce"] = _nonce;
    j["height"] = _height;
    j["timestamp"] = _timestamp;
    //j["hash"] = "";
    j["txcount"] = _transactions.size();
    j["prevhash"] = _prev_hash;

    json txs = json::array();
    for (auto &tx : _transactions)
        txs.push_back(json::parse(tx.to_string()));

    j["txs"] = txs;
    return j.dump(2);
}

block block::from_string(const std::string &string)
{
    block bl;
    json j = json::parse(string);
    bl._nonce = j["nonce"];
    bl._height = j["height"];
    bl._timestamp = j["timestamp"];
    for (auto &item : j["txs"].array())
    {
        bl._transactions.push_back(transaction::from_string(item));
    }
    return bl;
}

std::string block::calc_hash() const
{
    std::stringstream buffer;
    buffer << to_string();

    for (const auto &tx : _transactions)
        buffer << tx.to_hash();

    return sha256(buffer.str());
}

void block::update_time_now()
{
    _timestamp = system_clock::to_time_t(system_clock::now());
}

block block::genesis()
{
    const std::string pk = "0431adaf38ba920405bbdb788cb5018027e31c4ad002563f5b8410a855540083b2cd6fc850f3062a3774b65d2754545d61027ca8af3ad5978f1be36e7e57c610ef";
    block genesis;
    genesis.set_prev_hash("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    genesis.set_height(0);
    transaction tx;
    tx._inputs.push_back(input_t{1000, "FFFFF"});
    tx._outputs.push_back(output_t{1000, pk});
    genesis.add(tx);
    genesis.save_tx_data();
    genesis._hash = genesis.calc_hash();
    return genesis;
}

void block::save_tx_data()
{
    std::vector<json> txs;
    for (const auto& tx : _transactions)
        txs.push_back(json::parse(tx.to_string()));

    std::string data = json(txs).dump();

    auto encoded = base16_encode(std::vector<unsigned char>{data.begin(), data.end()});
    _txdata = std::string{encoded.begin(), encoded.end()};
}

void block::load_tx_data()
{
    auto decoded = base16_decode({_txdata.begin(), _txdata.end()});
    auto string = std::string{decoded.begin(), decoded.end()};
    json json = json::parse(string);
    for (auto &value : json)
    {
        _transactions.push_back(transaction::from_string(value.dump()));
    }
}

}
