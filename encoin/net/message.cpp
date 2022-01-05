#include "message.h"

#include <net/node.h>

namespace encoin {

json new_tx_message::handle_response(node *node, nlohmann::json request)
{
    auto tx = transaction::from_string(request["payload"]);
    std::cout << "new transaction received" << std::endl;
    node->chain().push(tx);
    return {};
}
std::string new_tx_message::build_request(const transaction &tx)
{
    return tx.to_string();
}

json new_block_message::handle_response(node *node, nlohmann::json request)
{
    auto bl = block::from_string(request["payload"]);
    std::cout << "new block received" << std::endl;
    node->chain().push(bl);
    return {};
}
std::string new_block_message::build_request(const block &bl)
{
    return bl.to_string();
}


json get_pool_message::handle_response(node *node, nlohmann::json request)
{
    std::vector<std::string> txs;
    for (auto &tx : node->chain().pool())
    {
        txs.push_back(tx.to_string());
    }
    return txs;
}
tx_list get_pool_message::parse_result(nlohmann::json j)
{
    tx_list list;
    for (auto &it : j)
    {
        list.push_back(transaction::from_string(it));
    }
    return list;
}

json get_latest_block_message::handle_response(node *node, nlohmann::json request)
{
    return node->chain().last_block().to_string();
}
block get_latest_block_message::parse_result(nlohmann::json j)
{
    return block::from_string(j);
}

json get_chain_message::handle_response(node *node, nlohmann::json request)
{
    std::vector<std::string> list;
    for (auto &block : node->chain().blocks())
    {
        list.push_back(block.to_string());
    }
    return json(list);
}
block_list get_chain_message::parse_result(nlohmann::json j)
{
    block_list list;
    for (auto &it : j)
    {
        list.push_back(block::from_string(it));
    }
    return list;
}

}
