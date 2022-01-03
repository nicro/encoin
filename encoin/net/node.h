#ifndef NODE_H
#define NODE_H

#include <net/peer.h>
#include <thread>
#include <vector>
#include <nlohmann/json.hpp>
#include <blockchain.h>
#include <variant>

namespace encoin {

using nlohmann::json;

class node {
public:

    node(blockchain&, std::string addr = "127.0.0.1", unsigned short port = 1234);
    ~node();

    void message_handler(tcp::socket socket);
    void server_loop();

    std::string on_message(const std::string &msg);
    void on_close();

    void add_peer(const std::string &addr, unsigned short port);
    peer &get_peer(const std::string &host, unsigned short port);
    bool has_peer(const std::string &host, unsigned short port);
    peer &get_peer(const std::string &host);
    bool has_peer(const std::string &host);

    std::vector<peer> &peers() { return _peers; }
    blockchain &chain() { return _chain; }

    void run_server();
    void wait_server();

    void add_peers_from_env();

    template <class msg_type>
    inline std::string prepare_msg(typename msg_type::arg_type data = {})
    {
        json j;
        auto msg = msg_type();
        j["type"] = msg.type();
        j["payload"] = msg.make_request(this, data);
        j["address"] = _address;
        j["port"] = _port;
        return j.dump();
    }

    template <class msg_type>
    inline std::string dispatch(typename msg_type::arg_type data = {})
    {
        if (!_peers.size()) return "";
        return _peers.front().send(prepare_msg<msg_type>(data));
    }

    template <class msg_type>
    inline void broadcast(typename msg_type::arg_type data = {})
    {
        auto request = prepare_msg<msg_type>(data);
        for (auto &peer : _peers) peer.send(request);
    }

public:
    blockchain &_chain;
    std::vector<peer> _peers;
    net::io_context _ctx;
    std::thread _srv_thread;
    std::string _address;
    unsigned short _port;
};

}

#endif // NODE_H
