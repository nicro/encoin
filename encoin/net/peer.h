#ifndef PEER_H
#define PEER_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace encoin {

class node;
class peer;

typedef std::vector<peer> peer_list;

class peer {
public:
    using wsstream_t = websocket::stream<tcp::socket>;

    peer(net::io_context &ctx, const std::string &host, unsigned short port)
    : _ctx(ctx), _socket(ctx), _host(host), _port(port) {}

    void connect();
    std::string send(const std::string &text);
    void close();

    std::string to_string() const;
    unsigned short port() const { return _port; }
    std::string host() const { return _host; }

    static std::string send(wsstream_t &socket, const std::string &text);

protected:
    unsigned short _port;
    std::string _host;
    net::io_context &_ctx;
    wsstream_t _socket;
};

}

#endif // PEER_H
