#include <net/node.h>
#include <iostream>
#include <cstdlib>
#include <sstream>

namespace encoin {

node::node(unsigned short port)
    : _port(port)
{
    add_peers_from_env();
    for (auto &peer : _peers)
    {
        peer.connect();
    }
}

void node::add_peers_from_env()
{
    // Format: peers=127.0.0.1:86;127.0.0.1:87
    if(const char *peers = std::getenv("peers"))
    {
        std::istringstream stream(peers);
        std::string new_peer;
        while (getline(stream, new_peer, ';'))
        {
            if (std::size_t del = new_peer.find(":"); del != std::string::npos)
            {
                std::string addr = new_peer.substr(0, del);
                std::string port = new_peer.substr(del + 1);
                add_peer(addr, std::stoi(port));
            }
        }
    }
}

node::~node()
{
    for (auto &peer : _peers)
    {
        peer.close();
    }
}

void node::run_server()
{
    _srv_thread = std::thread(&node::server_loop, this);
}

void node::wait_server()
{
    _srv_thread.join();
}

std::string node::on_message(const std::string &msg)
{
    return msg + " someotherdata";
}

void node::message_handler(tcp::socket socket)
{
    try
    {
        websocket::stream<tcp::socket> ws{std::move(socket)};

        ws.accept();
        for(;;)
        {
            beast::flat_buffer buffer;
            ws.read(buffer);
            ws.text(ws.got_text()); // set text mode if needed

            std::string reqstr = beast::buffers_to_string(buffer.data());
            ws.write(net::buffer(on_message(reqstr)));
        }
        on_close();
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
            std::cout << "waiting for requests..." << std::endl;
            acceptor.accept(socket);
            std::thread(&node::message_handler, this, std::move(socket)).detach();
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
