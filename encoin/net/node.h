#ifndef NODE_H
#define NODE_H

#include <thread>
#include <vector>
#include <nlohmann/json.hpp>
#include <net/peer.h>
#include <blockchain.h>

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

    peer_list &peers() { return _peers; }
    blockchain &chain() { return _chain; }

    void run_server();
    void wait_server();

    void add_peers_from_env();

    template <class msg_type>
    inline typename msg_type::rtype dispatch()
    {
        auto msg = msg_type();

        json j;
        j["type"] = msg.type();
        j["address"] = _address;
        j["port"] = _port;
        j["is_broadcast"] = false;

        if (_peers.size() == 0)
            return {};

        std::string request = j.dump();
        std::string response = _peers.front().send(request);
        json result = json::parse(response);
        return msg.parse_result(result["payload"]);
    }

    template <class msg_type>
    inline void broadcast(typename msg_type::ptype data)
    {
        auto msg = msg_type();

        json j;
        j["type"] = msg.type();
        j["payload"] = msg.build_request(data);
        j["is_broadcast"] = true;

        std::string request = j.dump();
        for (auto &peer : _peers)
        {
            //std::cout << "broadcasting to peer on port " << peer.port() << std::endl;
            peer.send(request);
        }
    }

public:
    blockchain &_chain;
    peer_list _peers;
    net::io_context _ctx;
    std::thread _srv_thread;
    std::string _address;
    unsigned short _port;
};

}

#endif // NODE_H
