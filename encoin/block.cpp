#include "block.h"
#include <nlohmann/json.hpp>

namespace encoin {

using std::chrono::system_clock;
using nlohmann::json;

block::block()
{
    update_time_now();
}

void block::add(const transaction &tx)
{
    _transactions.push_back(tx);
}

std::string block::calc_hash() const
{
    json j;
    j["nonce"] = _nonce;
    j["height"] = _height;
    j["timestamp"] = _timestamp;
    //j["hash"] = "";
    j["txcount"] = _transactions.size();
    j["prevhash"] = _prev_hash;

    return j.dump();
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
    transaction tx;
    tx._inputs.push_back(input_t{1000, "FFFFF"});
    tx._outputs.push_back(output_t{1000, pk});
    genesis.add(tx);
    return genesis;
}

}

