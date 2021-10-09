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
    block genesis;
    genesis.set_prev_hash("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    transaction tx{};
    //genesis.add({ "satoshi", "nicro", 50 });
    return genesis;
}

}

