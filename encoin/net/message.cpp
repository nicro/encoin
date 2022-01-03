#include <net/message.h>
#include <net/node.h>

namespace encoin {

json new_tx_message::make_response(node *node, nlohmann::json request)
{
    auto tx = transaction::from_string(request["payload"]);
    std::cout << "new transaction received" << std::endl;
    node->chain().push(tx);
    return {};
}
json new_tx_message::make_request(node *node, const transaction &tx)
{
    return tx.to_string();
}

json new_block_message::make_response(node *node, nlohmann::json request)
{
    auto bl = block::from_string(request["payload"]);
    std::cout << "new block received" << std::endl;
    node->chain().push(bl);
    return {};
}
json new_block_message::make_request(node *node, const block &bl)
{
    return bl.to_string();
}

json get_peers_message::make_response(node *node, nlohmann::json request)
{
    std::vector<std::string> peers;
    for (auto &peer : node->peers())
    {
        peers.push_back(peer.to_string());
    }
    return peers;
}

json get_pool_message::make_response(node *node, nlohmann::json request)
{
    std::vector<std::string> txs;
    for (auto &tx : node->chain().pool())
    {
        txs.push_back(tx.to_string());
    }
    return txs;
}

json get_latest_block_message::make_response(node *node, nlohmann::json request)
{
    return node->chain().last_block().to_string();
}

json get_chain_message::make_response(node *node, nlohmann::json request)
{
    return node->chain().to_string();
}

}
