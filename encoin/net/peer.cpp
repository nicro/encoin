#include <net/peer.h>
#include <iostream>
#include <net/node.h>

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
    }
    catch (std::exception const &e)
    {
        std::cerr << "error on connect: " << e.what() << std::endl;
    }
}

std::string peer::send(const std::string &text)
{
    return peer::send(_socket, text);
}

std::string peer::send(wsstream_t &socket, const std::string &text)
{
    try
    {
        beast::flat_buffer buffer;
        socket.write(net::buffer(text));
        socket.read(buffer);
        return beast::buffers_to_string(buffer.data());
        //std::cout << "client response: " << beast::make_printable(buffer.data()) << std::endl;
    }
    catch (const beast::system_error &error)
    {
        if(error.code() != websocket::error::closed)
            std::cerr << "Send error: " << error.code().message() << std::endl;
        return "";
    }
}

void peer::close()
{
    if (_socket.is_open())
    {
        _socket.close(websocket::close_code::normal);
    }
}

std::string peer::to_string() const
{
    return _host + ":" + std::to_string(_port);
}

}
