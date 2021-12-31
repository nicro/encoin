#ifndef NODE_H
#define NODE_H

#include <net/peer.h>
#include <thread>
#include <vector>
#include <nlohmann/json.hpp>

namespace encoin {

using nlohmann::json;

class node {
public:

    node(unsigned short port = 80);
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

    void run_server();
    void wait_server();


    void add_peers_from_env();

public:
    std::vector<peer> _peers;
    net::io_context _ctx;
    std::thread _srv_thread;
    unsigned short _port;
};

}

#endif // NODE_H
