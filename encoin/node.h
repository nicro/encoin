#ifndef NODE_H
#define NODE_H

#include <peer.h>
#include <thread>
#include <vector>

namespace encoin {

class node {
public:

    node(unsigned short port = 80) : _port(port) { }

    static void on_message(tcp::socket socket);
    void server_loop();

    void add_peer(const std::string &addr, unsigned short port);
    peer &get_peer(const std::string &host, unsigned short port);
    bool has_peer(const std::string &host, unsigned short port);
    peer &get_peer(const std::string &host);
    bool has_peer(const std::string &host);

    std::vector<peer> &peers() { return _peers; }

    std::thread create_server_loop();

public:
    std::vector<peer> _peers;
    net::io_context _ctx;
    unsigned short _port;
};

}

#endif // NODE_H
