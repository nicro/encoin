#ifndef NODE_H
#define NODE_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <thread>
#include <chrono>

namespace encoin {

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef server::message_ptr message_ptr;

class node {
public:
    node(unsigned port);

    void add_peer(const std::string &uri);
    void connect_peers();
    void broadcast(const std::string &message);

    void run_server();
    void run_client();
    void run_async();
    void await();

    void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);
    void on_open(server* s, websocketpp::connection_hdl hdl);

protected:
    unsigned _port;
    client _client;
    server _server;
    std::thread _client_thread, _server_thread;
    std::vector<client::connection_ptr> _peers;
};

}
#endif // NODE_H
