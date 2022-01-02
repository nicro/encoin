#include <net/message.h>
#include <net/node.h>

namespace encoin {

json new_tx_message::make_response(node *node, nlohmann::json request)
{
    auto tx = transaction::from_string(get_payload(request));
    node->chain().push(tx);
    return {};
}
json new_tx_message::make_request(node *node, const transaction &tx)
{
    json j;
    set_type(j, type());
    set_payload(j, tx.to_string());
    return j;
}

json new_block_message::make_response(node *node, nlohmann::json request)
{
    auto bl = block::from_string(get_payload(request));
    node->chain().push(bl);
    return {};
}
json new_block_message::make_request(node *node, const block &bl)
{
    json j;
    set_type(j, type());
    set_payload(j, bl.to_string());
    return j;
}

json get_peers_message::make_response(node *node, nlohmann::json request)
{
    json j;
    set_type(j, type());

    std::vector<std::string> peers;
    for (auto &peer : node->peers())
    {
        peers.push_back(peer.to_string());
    }
    set_payload(j, peers);
    return j;
}
json get_peers_message::make_request(node *node, const std::monostate&)
{
    json j;
    set_type(j, type());
    return j;
}

json get_pool_message::make_response(node *node, nlohmann::json request)
{
    json j;
    set_type(j, type());
    std::vector<std::string> txs;
    for (auto &tx : node->chain().pool())
    {
        txs.push_back(tx.to_string());
    }
    set_payload(j, txs);
    return j;
}
json get_pool_message::make_request(node *node, const std::monostate&)
{
    json j;
    set_type(j, type());
    return j;
}

json get_latest_block_message::make_response(node *node, nlohmann::json request)
{
    json j;
    set_type(j, type());
    set_payload(j, node->chain().last_block().to_string());
    return j;
}
json get_latest_block_message::make_request(node *node, const std::monostate &)
{
    json j;
    set_type(j, type());
    return j;
}

json get_chain_message::make_response(node *node, nlohmann::json request)
{
    json j;
    set_type(j, type());
    set_payload(j, node->chain().to_string());
    return j;
}
json get_chain_message::make_request(node *node, const std::monostate &)
{
    json j;
    set_type(j, type());
    return j;
}

}
