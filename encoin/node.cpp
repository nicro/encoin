#include "node.h"
#include <iostream>

namespace encoin {

std::thread node::create_server_loop()
{
    return std::thread(&node::server_loop, this);
}

void node::on_message(tcp::socket socket)
{
    try {
        websocket::stream<tcp::socket> ws{std::move(socket)};
        ws.accept();
        for(;;)
        {
            beast::flat_buffer buffer;
            ws.read(buffer);
            ws.text(ws.got_text()); // set text mode if needed
            ws.write(buffer.data()); // echo
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
}

void node::server_loop() 
{
    try {
        tcp::acceptor acceptor{_ctx, {net::ip::make_address("127.0.0.1"), _port}};
        for(;;)
        {
            tcp::socket socket{_ctx};
            acceptor.accept(socket);
            std::thread(&node::on_message, std::move(socket)).detach();
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
}

void node::add_peer(const std::string &addr, unsigned short port) 
{
    _peers.push_back(peer(_ctx, addr, port));
}

peer &node::get_peer(const std::string &host, unsigned short port)
{
    for (auto &peer : _peers)
    {
        if (peer.host() == host && peer.port() == port)
        {
            return peer;
        }
    }
    return _peers.front();
}

bool node::has_peer(const std::string &host, unsigned short port)
{
    for (auto &peer : _peers)
    {
        if (peer.host() == host && peer.port() == port)
        {
            return true;
        }
    }
    return false;
}

peer &node::get_peer(const std::string &host)
{
    for (auto &peer : _peers)
    {
        if (peer.host() == host)
        {
            return peer;
        }
    }
    return _peers.front();
}

bool node::has_peer(const std::string &host)
{
    for (auto &peer : _peers)
    {
        if (peer.host() == host)
        {
            return true;
        }
    }
    return false;
}

}
