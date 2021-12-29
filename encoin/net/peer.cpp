#include <net/peer.h>
#include <iostream>

namespace encoin {

void peer::connect() 
{
    try
    {
        tcp::resolver resolver{_ctx};
        auto const results = resolver.resolve(_host, std::to_string(_port));
        auto ep = net::connect(_socket.next_layer(), results);
        _host += ':' + std::to_string(ep.port());
        _socket.handshake(_host, "/");
        on_connect();
    }
    catch (std::exception const& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
}

void peer::on_connect()
{
    this->send("Hello :)");
}

void peer::send(const std::string &text) 
{
    beast::flat_buffer buffer;
    _socket.write(net::buffer(std::string(text)));
    _socket.read(buffer);
    std::cout << "client response: " << beast::make_printable(buffer.data()) << std::endl;
}

void peer::close()
{
    if (_socket.is_open())
    {
        _socket.close(websocket::close_code::normal);
    }
}

}
