#include "node.h"

namespace encoin {

node::node(unsigned port)
    : _port(port), _client(), _server(), _peers()
{
    _client.set_access_channels(websocketpp::log::alevel::none);
    _server.set_access_channels(websocketpp::log::alevel::none);

    _client.init_asio();
    _server.init_asio();

    _server.set_message_handler(bind(&encoin::node::on_message, this, &_server, _1, _2));
    _server.set_open_handler(bind(&node::on_open, this, &_server, _1));
}

void node::connect_peers()
{
    for (auto &peer : _peers)
    {
        _client.connect(peer);
    }
}

void node::broadcast(const std::string &message)
{
    for (auto &peer : _peers)
    {
        websocketpp::lib::error_code error;
        _client.send(peer->get_handle(), message, websocketpp::frame::opcode::value::text, error);
        if (error)
        {
            std::cout << error.message() << std::endl;
        }
    }
}

void node::run_client()
{
    _client.run();
}

void node::run_async()
{
    _server_thread = std::thread(&node::run_server, this);
    _client_thread = std::thread(&node::run_client, this);
}

void node::await()
{
    _server_thread.join();
    _client_thread.join();
}

void node::on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "message: " << msg->get_payload() << std::endl;
}

void node::on_open(server *s, websocketpp::connection_hdl hdl)
{
    std::cout << "connection opened" << std::endl;
}

void node::run_server()
{
    _server.listen(_port);
    _server.start_accept();
    _server.run();
}

void node::add_peer(const std::string &uri)
{
    try
    {
        websocketpp::lib::error_code error;
        auto new_peer = _client.get_connection(uri, error);
        if (error)
        {
            std::cout << error.message() << std::endl;
        }
        _peers.push_back(new_peer);
    }
    catch (...)
    {
        std::cout << "unexpected error" << std::endl;
    }
}

}
